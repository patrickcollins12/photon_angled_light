

function readParticleVariable(variableName, resultFunction) {
  var deviceID = "38003b001347363336383437";
  var accessToken = "82a528ad4ec2c1d91f74d4155e4ce1e8e41cebd2";

  requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + variableName
  var formData = {
    'access_token': accessToken,
  }

  $.getJSON(requestURL, formData, function (json) {
    console.log(json)
    resultFunction(json.result);
  });
}


function testHandler(result) {
  console.log("Value: ", result)
}

function setup() {
  // TODO: Alexa
  // TODO: More fun modes
  // TODO: test out defaults and disabled or slow wifi

  // when first loading try to get the mode from the photon
  readParticleVariable("mode",modeVariableResultHandler);

  // when first loading try to get the brightness from the photon
  readParticleVariable("brightness",brightnessVariableResultsHandler);


  // setup the color picker in the DOM
  var colorWell = document.querySelector("#colorWell");
  colorWell.value = defaultColor;
  colorWell.addEventListener("change", updateColor, false);
  colorWell.select();

  // setup the white slider in the DOM
  var whiteSlider = document.querySelector("#white");
  whiteSlider.addEventListener("change", updateWhite, false);

  // setup the brightness slider in the DOM
  var brightnessSlider = document.querySelector("#brightness");
  brightnessSlider.addEventListener("change", updateBrightness, false);
  
  // setup the mode radio handler in the DOM
  var radios = document.getElementsByName("mode");
  for (var i = 0; i < radios.length; i++) {
    console.log(i)
    radios[i].addEventListener("change", handleModeClick, false);
  }

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

// HANDLE BRIGHTNESS LOADING AND CLICK EVENTS 
function brightnessVariableResultsHandler(value) {
  document.querySelector("#brightness").value = value;
}

function updateBrightness(event) {
  callParticleFunction("brightness", event.target.value);
}

// HANDLE MODE LOADING AND CLICK EVENTS 
function modeVariableResultHandler(mode) {
  console.log("mode:",mode);
  var radios = document.getElementsByName("mode");
  for (var i = 0; i < radios.length; i++) {
    r = radios[i];
    r.checked = (r.value == mode)?true:false;
  }

  displayColorSection(mode);
}

function handleModeClick(event) {
  console.log("here")
  var mode = event.target.value
  console.log("Mode:", mode);
  callParticleFunction("mode", mode);
  displayColorSection(mode);
}

// hide or display the color section
function displayColorSection(mode) {
    // show or hide the color section

    var c = document.querySelector("#color_section");
    if (mode == "color") {
      readParticleVariable("rgbw_value",rgbwVariableResultHandler);
      c.style.display = "inline"
    } else {
      c.style.display = "none"
    }
}

function rgbwVariableResultHandler(rgbw) {

  // decompact the int into four parts
  var r = (rgbw >> 24) & 0xFF;
  var g = (rgbw >> 16) & 0xFF;
  var b = (rgbw >> 8) & 0xFF;
  var w = rgbw & 0xFF;

  // retrieved the color from the photon, set the swatch
  let c = colorutil.color({r:r,g:g,b:b});
  console.log(c.rgb, c.hex);
  document.querySelector("#colorWell").value = c.hex; 
  
  // retrieved the whiteness from the photon, set the slider
  document.querySelector("#white").value = w;  

}
