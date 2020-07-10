## oneString

A simple DIY usb ribbon synthesizer controller

This project was originally released on May 15, 2012. It is available here for reference. Though updates are no longer being created, I will support builders and kit owners as possible.

#### Description
Inspired by the simplicity of the one-string “Diddley-Bow” of Delta Blues, the oneString controller is a super-simple and highly-expressive controller for laptop musicians. It’s designed to be an easy to construct, open-source USB MIDI ribbon controller for musicians looking for a new way to perform and compose.

<ul>
<li class="level1"><div class="li">USB Powered, allowing for only one cable. This means easy setup and portability in our wire-filled lives.</div>
</li>
<li class="level1"><div class="li">Uses a standard MIDI signal over USB allowing compatibility with almost all music software packages. (ANYTHING that takes keyboard input: Reason, reNoise, Energy XT, Ableton, FL Studio, Max-MSP, PureData… the list is endless!)</div>
</li>
<li class="level1"><div class="li">Uses free and open-source drivers to allow the most economical cross-platform (WIN, OSX, LINUX) solution.</div>
</li>
<li class="level1"><div class="li">Uses a minimum of parts, making it cheap and allowing for simple build instruction that even a beginner builder can finish in a day.</div>
</li>
<li class="level1"><div class="li">Body construction based on simple parts, allowing for everyone to easily make cool unique instruments without a wood-shop or plastics prototyping shop at their disposal.</div>
</li>
<li class="level1"><div class="li">Uses Arduino firmware allowing for augmentation and support from a HUGE open-source community.</div>
</li>
</ul>
Check out the demo on Youtube! : http://youtu.be/1mFMLxMdEss

#### How it Works – What’s a “ribbon controller”??

A “ribbon controller” is a long position-sensing strip that can control the tone or modulation of a synthesizer, much like a slide-able guitar string. The sensor is read by a condensed USB powered “Arduino” circuit, which sends the MIDI synth signal to your laptop. Once there, the signal is translated by open-source software and transported to the music software of your choice.

The device has a dip-switch to allow for 16 preset interface modes (ex: pitch-bend continuous tone, individual note sending, multi-octave ranges, control of other MIDI “CC’s, etc… ). A potentiometer (knob) allows the user to change the tuning of the instrument on the fly. Unlike currently available ribbon controllers, the oneString can be triggered by a second “rhythm tap” sensor, allowing for a unique 2-handed play-style.


#### Files

**oneString_Build_May24.pdf** : The original kit building instructions  
**oneString_GettingStarted.pdf** : The original Software instructions  
**oneString.ino** : The original Arduino firmware for the oneString controller  
**oneString_Schematic.png**: Schematic for the synth  
**Modifications.md** : Information for advanced modifications to the instrument
**LICENSE** : GPL-3.0 License  
**README.md** : This file
**libraries (directory)** : the ardumidi library and original license

#### Useful Links
[Hairless Midi](https://github.com/projectgus/hairless-midiserial): A cross platform MIDI/Serial bridge

[ftdichip.com](https://www.ftdichip.com/Drivers/VCP.htm): The FTDI drivers
