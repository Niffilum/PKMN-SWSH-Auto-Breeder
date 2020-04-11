# required software
WinAVR: http://winavr.sourceforge.net/download.html

- make sure add directories to PATH is enabled during installation

board software

- arduino: https://www.microchip.com/developmenttools/ProductDetails/flip

- teensy: https://www.pjrc.com/teensy/loader_win10.html

# auto hosters

### `softlockHoster.hex`
this will allow you to auto host the same pokemon **without** losing your rolling den
- roll to your desired shiny frame, face the den
- move your cursor over the Pokemon Camp option in the menu (***required step***, explanation [here](#soft-lock-explanation))
- close the menu
- navigate to change grip/order menu (switch home -> controllers)
- plug in your arduino

### `rollHoster.hex`
this will roll 3 frames forward, host the den & repeat
- save in front of the den, facing it
- navigate to change grip/order menu (switch home -> controllers)
- plug in your arduino

##### (link code versions later)

## soft lock explanation
this method utilizes the backup save in swsh.
multiple things in the game create a backup save, such as saving, trading, camping & flying.
by camping, your backup save will contain the pokemon you rolled on your shiny frame, while your original save will be untouched.

other uses include:
- closing the game & preserving your soft lock 
- hosting locally while soft locking
- keeping a pokemon soft locked while rolling for something else 

## other useful tools created by [brianuuuSonic](https://www.youtube.com/watch?v=y2xFf7e_KSU)

- autohost (hardlock)
- day skipper (frame skipping)
- 3 day skipper (rolling den)
- autofossil 
- autoloto (masterball farming)
- watt farmer
- turboA (digging bros etc.)
- berry farmer
- auto box release


## thanks & credits
[progmem](https://github.com/progmem/Switch-Fightstick)

[shinyquagsire23](https://github.com/shinyquagsire23/Switch-Fightstick)

[brianuuuSonic](https://www.youtube.com/user/brianuuusonic2)

bgoproton & svatinus 
