rm -rf frames*.pgm

gcc doomfire.c && ./a.out 256 256 128 && convert frame_*.pgm -filter point -resize 200% frames.gif && popup
