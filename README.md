# sxlb

Using this is very easy (on GNU/Linux), you only need the Assembler NASM and the Emulator Bochs.

Qemu maybe works too, but I will explain it here for Bochs.

Attention! You are using this guide on own risk.
I'm not to blame, if this harms your system.

##Downloading

###sxlb
`git clone https://github.com/ggwpez/sxlb.git`

And open it us with: `cd sxlb`

###NASM
`sudo apt-get install nasm`

###Bochs
`sudo apt-get install bochs`

You maybe also need 'bochs-sdl' since the contained configuration file 'bochsrc' implicates this.

`sudo apt-get install bochs-sdl`

##Testing
Make it with:
`./make`

And start it with:
`bochs`

Or if you want both at once, use:
`./start`
