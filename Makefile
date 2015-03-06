all:
	if [ ! -d unicorn-hat ]; then make lib; else echo "unicorn-hat directory exists, skipping make lib..."; fi
	make invaders 

lib:    
	git clone https://github.com/pimoroni/unicorn-hat
	make -C unicorn-hat/python/rpi-ws281x/lib

clean:
	-rm -f invaders

reallyclean:
	-rm -rf invaders unicorn-hat

invaders:
	gcc -Wall invaders.c -o invaders -I unicorn-hat/python/rpi-ws281x/lib -L unicorn-hat/python/rpi-ws281x/lib -lm -lws2811
