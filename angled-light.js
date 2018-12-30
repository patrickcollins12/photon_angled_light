function readParticleVariable(variableName, resultFunction) {
  var deviceID = "38003b001347363336383437";
  var accessToken = "82a528ad4ec2c1d91f74d4155e4ce1e8e41cebd2";

  requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variableName
  var formData = {
    'access_token': accessToken,
  }

  $.post(requestURL, formData, function (json) {
    console.log(json)
    resultFunction(json.result);
  });
}

function callParticleFunction(functionName, argValue) {
  var deviceID = "38003b001347363336383437";
  var accessToken = "82a528ad4ec2c1d91f74d4155e4ce1e8e41cebd2";

  requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + functionName
  var formData = {
    'access_token': accessToken,
    'arg': argValue
  }

  $.post(requestURL, formData, function (json) {
    console.log(json)
  });
}

// var colorWell;
// var whiteSlider;

var defaultColor = "#000000";

function setup() {
  // TODO: read the values
  // TODO: Alexa
  // TODO: More fun modes
  //   readParticleVariable(variableName,function { console.log("this is the value")});

  // setup the color picker
  var colorWell = document.querySelector("#colorWell");
  colorWell.value = defaultColor;
  colorWell.addEventListener("change", updateColor, false);
  colorWell.select();

  // setup the white slider
  var whiteSlider = document.querySelector("#white");
  whiteSlider.addEventListener("change", updateWhite, false);

  // TODO: test out defaults and disabled or slow wifi
}

function updateColor(event) {
  colorValue = event.target.value;
  document.querySelector("#colorText").textContent = colorValue + " " + JSON.stringify(hexToRgb(colorValue));
  callParticleFunction("rgb_value", event.target.value);
}

function hexToRgb(hex) {
  var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? [
    parseInt(result[1], 16),
    parseInt(result[2], 16),
    parseInt(result[3], 16)
  ] : null;
}

function updateWhite(event) {
  callParticleFunction("w_value", event.target.value);
}

function handleModeChange(m) {
  var mode = m.value
  console.log("Mode:", mode);
  callParticleFunction("mode", mode);

  // show or hide the color section
  var c = document.querySelector("#color_section");
  if (mode == "color") {
    c.style.display = "inline"
  } else {
    c.style.display = "none"
  }

}