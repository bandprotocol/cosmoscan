[@bs.val] external document: Js.t({..}) = "document";

let style = document##createElement("style");
document##head##appendChild(style);
style##innerHTML #= AppStyle.style;

AxiosHooks.setRpcUrl(Env.rpc);

TimeAgos.setMomentRelativeTimeThreshold();

let setupSentry: unit => unit = [%bs.raw
  {|
function() {
  const Sentry = require("@sentry/browser");
  Sentry.init({dsn: "https://8e19a3df712d45ac9224384bbe66a7c5@o1127009.ingest.sentry.io/6168527"});
}
  |}
];
setupSentry();

ReactDOMRe.render(
  <ApolloClient>
    <ClientContext>
      <GlobalContext>
        <TimeContext>
          <ThemeContext>
            <ModalContext> <AccountContext> <Checker /> <App /> </AccountContext> </ModalContext>
          </ThemeContext>
        </TimeContext>
      </GlobalContext>
    </ClientContext>
  </ApolloClient>,
  document##getElementById("root"),
);
