
const deviceID = "290031001047363336383437";
const accessToken = "82a528ad4ec2c1d91f74d4155e4ce1e8e41cebd2";
var requestURL = `https://api.spark.io/v1/devices/${deviceID}`;
import axios from 'axios';

function setup() {

  // when first loading try to get the mode from the photon
  readParticleVariable("mode", modeVariableResultHandler);

  // when first loading try to get the brightness from the photon
  readParticleVariable("brightness",  function(value) {
    document.querySelector("#brightness").value = value;
  });  

  // when first loading try to get the schedule value from the photon
  readParticleVariable("schedule",  function(value) {
    document.querySelector("#schedule").checked = (value=="on")?true:false;
  });  
  
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

  // setup the party speed slider in the DOM
  // var psSlider = document.querySelector("#party_speed");
  // psSlider.addEventListener("change", updatePartySpeed, false);

  // party speed
  var psSlider = document.querySelector("#party_speed");
  psSlider.addEventListener("change", function(event) {
    callParticleFunction("party_speed", event.target.value);

  }, false);

  // color speed
  var s = document.querySelectorAll('input[name="color_speed"]');
  for (var i = 0, len = s.length; i < len; i++) {
    s[i].addEventListener("click", function(event) {
      var speed = event.target.value;
      callParticleFunction("color_speed", speed);
      document.querySelector("#color_speed_val").innerHTML = speed;
    });
  }

  // csSlider.addEventListener("change", function(event) {
  //   callParticleFunction("color_speed", event.target.value);

  // }, false);

  // setup schedule event listener
  var scheduleCheckbox = document.querySelector("#schedule");
  scheduleCheckbox.addEventListener("change", function(event) {
    callParticleFunction("schedule", (event.target.checked)?"on":"off");
  }, false);

  // setup the mode radio handler in the DOM
  var radios = document.getElementsByName("mode");
  for (var i = 0; i < radios.length; i++) {
    radios[i].addEventListener("change", handleModeClick, false);
  }

  
}

export default setup

// We're getting a variable (GETting) and inspecting the response
//  for the result param and posting to the resultFunction
function readParticleVariable(functionName, resultFunction) {
  axios(
    { url:    `${requestURL}/${functionName}`,
      params: { access_token: accessToken },
      method: 'GET',
      timeout: 9000
    })
  .then(function (response) {
    var result = response.data.result;
    console.log(`get ${functionName}:${result} (succeeded)` );
    resultFunction(result);
  })

}

// We're setting a variable (POSTING a function) 
// and ignoring the response
function callParticleFunction(functionName, arg) {
  axios(
    { url:    `${requestURL}/${functionName}`,
      params: { access_token: accessToken },
      method: 'POST',
      data:   { arg: arg },
      timeout: 9000
    })
  .then(function (response) {
    console.log(`set ${functionName}:${arg} (succeeded)`);
  })

}

function callParticleAxios(functionName, argName, resultFunction) {
  // We're setting a variable (POSTING a function) 
  // and ignoring the response
    axios(
      {
        url:    `${requestURL}/${functionName}`,
        method: 'POST',
        data:   { arg: argName },
        params: { access_token: accessToken }
      })
    .then(function (response) {
      console.log(`set ${argName} (succeeded)`);
    })
    .catch(function (error) {
      console.log(error);
    });
}

function callParticleFETCH(functionName, argName, resultFunction) {

  var requestURL = "https://api.spark.io/v1/devices/"
  requestURL += deviceID + "/" + functionName; 
  requestURL += "?access_token=" + accessToken;

  console.log("Calling particle: ", requestURL);
  var resp = fetch(requestURL, {
      mode: "cors",
      method: (argName) ? "POST" : "GET",
      body: (argName) ? JSON.stringify({
        arg: argName
      }) : null,
      headers: {
        'Content-Type': 'application/json'
      }
    })
    .then(response => {
      if (response.ok) {
        if (resultFunction) {
          response.text().then(function (text) {
            var result = JSON.parse(text).result
            console.log(functionName + ":", result)
            resultFunction(result);
          })

          // resultFunction(response.text().result);
        } else {
          console.log(response);
        }

      }
      // else {
      //   throw new Error(response.status + response.statusText);
      // }
    })
  // .catch( error => { return; console.error('Error:', error) }
  // );
}

var defaultColor = "#000000";

function updateColor(event) {
  var colorValue = event.target.value;
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

function updatePartySpeed(event) {
  callParticleFunction("party_speed", event.target.value);
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
  var radios = document.getElementsByName("mode");
  for (var i = 0; i < radios.length; i++) {
    var r = radios[i];
    r.checked = (r.value == mode)?true:false;
  }

  displaySubSection(mode);
}

function handleModeClick(event) {
  var mode = event.target.value
  callParticleFunction("mode", mode);
  displaySubSection(mode);
}

// hide or display the mode section
// and preload and variables set into the photon
function displaySubSection(mode) {

    if (mode=="party") {
      readParticleVariable("party_speed",function(speed) {
        document.querySelector("#party_speed").value = speed;
      });
    }

    if (mode=="colorcycle") {
      readParticleVariable("color_speed",function(speed) {
        document.querySelector("#color_speed_val").innerHTML = speed;
      });
    }

    if (mode=="color") {
      readParticleVariable("rgbw_value",rgbwVariableResultHandler);
    }

    // hide all the mode sections and show the 
    // right one for the mode
    const sections = document.querySelectorAll(".mode_section");
    for (var i = 0; i < sections.length; i++) {
      var s = sections[i];
      s.style.display = (s.id == mode + "_section" ) ? "inline" : "none";

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
