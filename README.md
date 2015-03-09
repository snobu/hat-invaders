# hat-invaders

### Display bitmap animation on Pimoroni's fantastic [Unicorn Hat](http://shop.pimoroni.com/products/unicorn-hat).

Frame animation from Phillip Burgess / Adafruit:<br />
https://learn.adafruit.com/trinket-slash-gemma-space-invader-pendant/animation

Written for the Pi2 Model B (should work on any Pi - i suppose - untested).

#### Q. "So, what exactly am i getting here?"
#### A. This: https://vine.co/v/OEFdgmg1TvX
[![invaders](http://i.imgur.com/Oa7rcOE.png)](https://vine.co/v/OEFdgmg1TvX)


#### <i>"YOUR COLORS ARE BAD AND YOU SHOULD FEEL BAD"</i>
Very well then, edit invaders.c:

```c
switch(mask) {
        case 128 : setPixelColorRGB(getPixelPosition(0,y), 166, 166, 42); break; /* yellow */
        case 64  : setPixelColorRGB(getPixelPosition(1,y),  52, 128, 42); break; /* green */
        case 32  : setPixelColorRGB(getPixelPosition(2,y),  52, 128, 42); break;
        ...
```

While in here look for frame duration, change my 6000 to something bigger if you want to slow it down.

### INSTALLATION
```bash    
git clone https://github.com/snobu/hat-invaders
cd hat-invaders
make
```

### RUN
```bash
./invaders
```

### I CAN'T EVEN
You can. CTRL + C to stop.
