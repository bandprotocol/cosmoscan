let style = {j|
html,
body,
div,
span,
applet,
object,
iframe,
h1,
h2,
h3,
h4,
h5,
h6,
p,
blockquote,
pre,
a,
abbr,
acronym,
address,
big,
cite,
code,
del,
dfn,
em,
img,
ins,
kbd,
q,
s,
samp,
small,
strike,
strong,
sub,
sup,
tt,
var,
b,
u,
i,
center,
dl,
dt,
dd,
ol,
ul,
li,
fieldset,
form,
label,
legend,
table,
caption,
tbody,
tfoot,
thead,
tr,
th,
td,
article,
aside,
canvas,
details,
embed,
figure,
figcaption,
footer,
header,
hgroup,
menu,
nav,
output,
ruby,
section,
summary,
time,
mark,
audio,
video {
  margin: 0;
  padding: 0;
  border: 0;
  font-size: 100%;
  vertical-align: baseline;
}

/* Markdown Initial */

/* Typography */

p {
  font-size: 14px;
  line-height: 1.42;
}

a {
  transition: .3s;
}

h1,
h2,
h3,
h4,
h5,
h6 {
  line-height: 1.3;
  font-family: 'Montserrat', sans-serif;
}

h1 {
  font-size: 24px;
}
h2 {
  font-size: 20px;
}
h3 {
  font-size: 18px;
}
h4 {
  font-size: 14px;
}
h5 {
  font-size: 12px;
}


/* List */

ul > li + li {
  margin-top: 8px;
}


input {
  border: 0;
  margin: 0;
  padding: 0;
  -webkit-appearance: none;
  -moz-appearance: none;
  appearance: none;
}

/* HTML5 display-role reset for older browsers */
article,
aside,
details,
figcaption,
figure,
footer,
header,
hgroup,
menu,
nav,
section {
  display: block;
}

body {
  line-height: 1;
}

ol,
ul {
  list-style: none;
}

blockquote,
q {
  quotes: none;
}

blockquote:before,
blockquote:after,
q:before,
q:after {
  content: "";
  content: none;
}

table {
  border-collapse: collapse;
  border-spacing: 0;
}

* {
  box-sizing: border-box;
}

a:not(:hover) {
  text-decoration: none;
}

* {
  box-sizing: border-box;
  transition: color,background-color 200ms;
}

html,
body {
  max-width: 100vw;
  height: 100%;
}

body {
  margin: 0;
  padding: 0;
  display: flex;
  flex-direction: column;
  font-family: 'Montserrat', sans-serif;
  -moz-osx-font-smoothing: grayscale;
}

button, input {
  font-family: 'Montserrat', sans-serif;
    sans-serif;
}

code {
  font-family: source-code-pro, Menlo, Monaco, Consolas, "Courier New",
    monospace;
}

#root {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  height: 100%;
}

.container {
  width: 100%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 12px;
}

/* Select Input */

select {
  /* for Firefox */
  -moz-appearance: none;
  /* for Chrome */
  -webkit-appearance: none;
}

/* For IE10 */
select::-ms-expand {
  display: none;
}

/* scrolling smooth on ios */
* {
  -webkit-overflow-scrolling: touch;
}
|j};
