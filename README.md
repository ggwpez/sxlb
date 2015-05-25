# sxlb

Using this is very easy (on GNU/Linux), you only need the Assembler NASM, the Emulator Bochs and the GNU/g++ Compiler.

Qemu as Emulator maybe works too, but I will explain it here for Bochs.

Attention! You are using this guide on your own risk.
I'm not to blame, if this harms your system.
Use these commands only, if you are an advanced user and know what they do.

##Downloading

###git
`sudo apt-get install git`

###sxlb
`git clone https://github.com/ggwpez/sxlb.git`

Open the direcory with: `cd sxlb`

###NASM
`sudo apt-get install nasm`

###Bochs
`sudo apt-get install bochs`

You maybe also need 'bochs-sdl' since the contained configuration file 'bochsrc' implicates this.

`sudo apt-get install bochs-sdl`

###g++
`sudo apt-get install g++`

##Testing
Make it with:
`./make`

And start it with:
`bochs`

And if you want both at once:
`./start`
