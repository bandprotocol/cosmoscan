module Styles = {
  open Css;

  let card = (theme: Theme.t) =>
    style([
      backgroundColor(theme.secondaryBg),
      height(`percent(100.)),
      borderRadius(`px(8)),
      padding4(~top=`px(31), ~left=`px(32), ~right=`px(32), ~bottom=`px(10)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, `num(0.2)))),
      Media.mobile([padding2(~v=`px(24), ~h=`px(16))]),
    ]);

  let innerCard =
    style([width(`percent(100.)), height(`px(220)), margin2(~v=`zero, ~h=`auto)]);

  let requestCount = style([marginTop(`px(8))]);

  let chart = show => style([important(display(show ? `block : `none))]);
};

let renderGraph: (array(HistoricalTotalRequestQuery.t), bool) => unit = [%bs.raw
  {|
function(data, isDarkMode) {
  var Chart = require('chart.js');
  var ctx = document.getElementById('historicalRequest').getContext('2d');

  // change seconds to milliseconds
  data = data.map(({y, t}) => {
    return {
      y: y,
      t: t * 1000,
    }
  });

  var chart = new Chart(ctx, {
      // The type of chart we want to create
      type: 'line',

      // The data for our dataset
      data: {
        datasets: [{
            type: 'line',
            pointRadius: 0,
            fill: false,
            borderColor: isDarkMode ? '#ffffff' : '#303030',
            data: data,
            borderWidth: 2,
        }]
      },

      // Configuration options go here
      options: {
        maintainAspectRatio: false,
        legend: {
          display: false,
        },
        scales: {
          xAxes: [
            {
              type: 'time',
              distribution: 'series',
              gridLines: {
                display: false,
                drawBorder: false,
              },
              time: {
                unit: 'day',
                unitStepSize: 1,
                displayFormats: {
                  'day': 'MMM DD',
                },
              },
              ticks: {
                fontFamily: 'Inter',
                fontColor: isDarkMode ? '#888888' : '#7D7D7D',
                fontSize: 10,
                autoSkip: true,
                maxTicksLimit: 10,
              }
            },
          ],
          yAxes: [
            {
              gridLines: {
                display: true,
                color: isDarkMode ? '#353535' : '#EDEDED',
                drawBorder: false,
                zeroLineColor: isDarkMode ? '#353535' : '#EDEDED',
              },
              ticks: {
                fontFamily: 'Montserrat',
                fontColor: isDarkMode ? '#888888' : '#7D7D7D',
                fontSize: 10,
                maxTicksLimit: 5,
                callback: function(value) {
                  var ranges = [
                      { divider: 1e6, suffix: 'M' },
                      { divider: 1e3, suffix: 'K' }
                  ];
                  function formatNumber(n) {
                      for (var i = 0; i < ranges.length; i++) {
                        if (n >= ranges[i].divider) {
                            return (n / ranges[i].divider).toString() + ranges[i].suffix;
                        }
                      }
                      return n;
                  }
                  return formatNumber(value);
                }
              }
            },
          ],
        },
        tooltips: {
					mode: 'index',
					intersect: false,
          backgroundColor: '#000000',
          titleFontFamily: "Montserrat",
          titleFontSize: 14,
          titleFontColor: '#ffffff',
          titleFontStyle: "500",
          titleMarginBottom: 2,
          bodyFontFamily: "Montserrat",
          bodyFontSize: 10,
          bodyFontColor: '#888888',
          bodyFontStyle: "normal",
          xPadding: 15,
          yPadding: 13,
          caretSize: 6,
          displayColors: false,
          callbacks: {
            title: function(tooltipItem, data) {
              var title = (parseInt(tooltipItem[0].value)).toLocaleString();
              return title + " requests";
            },
            label: function(tooltipItem, data) {
              let date = new Date(tooltipItem.label);
              let dateTimeFormat = new Intl.DateTimeFormat('en', { year: 'numeric', month: 'short', day: '2-digit' });
              let [{ value: month },,{ value: day },,{ value: year }] = dateTimeFormat .formatToParts(date );

              return `${month} ${day},${year}`;
            },
          }
				},
      }
  });
}
  |}
];

[@react.component]
let make = () => {
  let dataQuery = HistoricalTotalRequestQuery.get();
  let (ThemeContext.{theme, isDarkMode}, _) = React.useContext(ThemeContext.context);
  let (lastCount, setLastCount) = React.useState(_ => 0);
  let (lastMode, setLastMode) = React.useState(_ => isDarkMode);
  let requestCountSub = RequestSub.count();

  React.useEffect2(
    () => {
      switch (dataQuery) {
      | Data(data) =>
        if (data->Belt.Array.size > 0) {
          // check the incoming data is a new data.
          let last = data->Belt.Array.get(data->Belt.Array.size - 1)->Belt.Option.getExn;

          // update when get new point
          if (last.y != lastCount) {
            setLastCount(_ => last.y);
            renderGraph(data, isDarkMode);
          };

          // update when change mode
          if (lastMode != isDarkMode) {
            setLastMode(_ => isDarkMode);
            renderGraph(data, isDarkMode);
          };
        }
      | _ => ()
      };
      None;
    },
    (dataQuery, isDarkMode),
  );

  <div className={Styles.card(theme)}>
    <div
      id="totalRequestsGraphSection"
      className={Css.merge([CssHelper.mb(~size=40, ()), CssHelper.mbSm(~size=16, ())])}>
      <div className={CssHelper.flexBox()}>
        <Heading value="Total Requests" size=Heading.H4 />
        <HSpacing size=Spacing.xs />
        <CTooltip tooltipText="The total number of oracle data requests made">
          <Icon name="fal fa-info-circle" size=10 color={theme.textSecondary} />
        </CTooltip>
      </div>
      <div className=Styles.requestCount>
        {switch (requestCountSub) {
         | Data(requestCount) =>
           <Heading
             value={requestCount |> Format.iPretty}
             size=Heading.H4
             weight=Heading.Thin
             color={theme.textSecondary}
           />
         | _ => <LoadingCensorBar width=65 height=21 />
         }}
      </div>
    </div>
    {switch (dataQuery) {
     | Data(data) =>
       let show = data->Belt.Array.size > 5;
       <div className=Styles.innerCard>
         <canvas id="historicalRequest" className={Styles.chart(show)} />
         <EmptyContainer display={!show} height={`percent(100.)}>
           <Icon name="fal fa-clock" size=40 color={isDarkMode ? theme.white : theme.black} />
           <VSpacing size={`px(16)} />
           <Heading
             size=Heading.H4
             value="Insufficient data to visualize"
             align=Heading.Center
             weight=Heading.Regular
             color={theme.textSecondary}
           />
         </EmptyContainer>
       </div>;
     | _ => <LoadingCensorBar fullWidth=true height=200 />
     }}
  </div>;
};
