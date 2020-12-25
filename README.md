# Photon Angled Light

I have built a wooden light sculpture out of walnut and epoxy resin. The light uses an embedded [Particle Photon microcontroller](https://docs.particle.io/photon/). The light has different modes and can be controlled by a web-based user interface.

This source repository contains:
 - the JS-based user interface for the remote control [which is hosted here](https://photon-angled.netlify.com).
 - the Arduino code to flash to the Particle Photon. 

## TODO
- More fun modes: throbbing, overlay one mode for another.
- color speed radio boxes don't select/update current value
- BUG: debug working on iphone
- add domain to netlify?
- make some auth to distribute to others
- loading status spinner and getting/setting spinner or debug readout
- Publish mode changes to UI? Would this need React? Meh couldn't be bothered.
- Make a base html home and serve from there. Already moved stuff to dist. https://stackoverflow.com/questions/27639005/how-to-copy-static-files-to-build-directory-with-webpack You don't need to copy things around, webpack works different than gulp. Webpack is a module bundler and everything you reference in your files will be included. You just need to specify a loader for that.

### DONE
- random mode
- get serial particle console logging going
- Alexa
- Move to materialize
- figure out how to build on netlify
- test out defaults and disabled or slow wifi
- party speed
- Moved Particle Photon to CLI and VSCode
- Set timeout on party mode
- github push from vscode
- More fun modes: white light dropping
- BUG: colorcycle seems to lock the device. maybe it is because the sleep cycle is too long?


## COMMANDS


npm install

`npm install`

production build

`npm run build`

dev watch

`npm run dev`

flash to the device

`npm run flash`

run the local webserver

`npm start`

debug particle 
`particle serial monitor`
or
`particle flash 290031001047363336383437 && sleep 7 && particle serial monitor`

To update netlify, just commit.