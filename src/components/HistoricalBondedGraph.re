module Styles = {
  open Css;

  let container =
    style([width(`percent(100.)), height(`px(250)), margin2(~v=`zero, ~h=`auto)]);

  let chart = show => style([important(display(show ? `block : `none))]);
};

let renderGraph: (array(HistoricalBondedQuery.t), bool) => unit = [%bs.raw
  {|
// TODO: let's binding chart.js later
function(data, isDarkMode) {
  var Chart = require('chart.js');
  var ctx = document.getElementById('historicalBonded').getContext('2d');

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
              ticks: {
                fontFamily: 'Inter',
                fontColor: isDarkMode ? '#888888' : '#7D7D7D',
                fontSize: 10,
                autoSkip: true,
                maxTicksLimit: 5,
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
                fontFamily: 'Inter',
                fontColor: isDarkMode ? '#888888' : '#7D7D7D',
                fontSize: 10,
                maxTicksLimit: 5,
                stepSize: 100000,
                callback: function(value) {
                  var ranges = [
                      { divider: 1e6, suffix: 'M' },
                      { divider: 1e3, suffix: 'K' }
                  ];
                  function formatNumber(n) {
                      for (var i = 0; i < ranges.length; i++) {
                        if (n >= ranges[i].divider) {
                            return (n / ranges[i].divider).toFixed(1) + ranges[i].suffix;
                        }
                      }
                      return n.toFixed(0);
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
          titleFontSize: 12,
          titleFontColor: '#ffffff',
          titleFontStyle: "500",
          titleMarginBottom: 2,
          bodyFontFamily: "Montserrat",
          bodyFontSize: 10,
          bodyFontColor: '#888888',
          bodyFontStyle: "normal",
          xPadding: 15,
          yPadding: 10,
          caretSize: 6,
          displayColors: false,
          callbacks: {
            title: function(tooltipItem, data) {
              var title = (parseInt(tooltipItem[0].value)).toLocaleString();
              return title + " Bonded BAND";
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
let make = (~operatorAddress) => {
  let dataQuery = HistoricalBondedQuery.get(operatorAddress);
  let (ThemeContext.{isDarkMode}, _) = React.useContext(ThemeContext.context);

  React.useEffect2(
    () => {
      switch (dataQuery) {
      | Data(data) => renderGraph(data, isDarkMode)
      | _ => ()
      };

      None;
    },
    (dataQuery, isDarkMode),
  );

  switch (dataQuery) {
  | Data(data) =>
    let show = data->Belt.Array.size > 5;

    <div className=Styles.container>
      <canvas id="historicalBonded" className={Styles.chart(show)} />
      <EmptyContainer height={`percent(100.)} display={!show}>
        <Icon name="fal fa-clock" size=40 color=Theme.primary_600 />
        <VSpacing size={`px(16)} />
        <Heading
          size=Heading.H4
          value="Insufficient data to visualize"
          align=Heading.Center
          weight=Heading.Regular
        />
      </EmptyContainer>
    </div>;
  | _ => <LoadingCensorBar.CircleSpin height=180 />
  };
};
