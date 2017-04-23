# Freescreenshots
Freescreenshots is a taiHEN plugin to literally free screenshots on your Vita. This plugin does two things:

1- It enables screenshots for some games/apps which block taking screenshots. PSP emu is one example to this.

2- It removes watermarks on screenshots some games/apps apply to screenshots.

How to use:

Put "freescreenshots.suprx" in 'tai' folder in the root of your Vita.
Change config.txt in that directory to load plugin for title of your choice by adding new lines like below:

titleid for your game (this one is for Super Stardust Delta for example to remove watermark on screenshots)

*PCSA00006
ux0:tai/freescreenshots.suprx

titleid for your game (this one is for Ape Escape Demo for example to enable screenshot for ARK)

*NPEG00005
ux0:tai/freescreenshots.suprx

After that, you can use PS button + START combo as usual to take screenshots. Now screenshots are enabled on every app/game and they won't be watermarked.

Most of the source code belong to yifanlu's screenshot disabling example. https://github.com/yifanlu/taiHEN/blob/master/USAGE.md#hooking-weak-imports

Thanks to dots_tb and frangar for helping me through and answering my stupid questions. Also TillusB's nowatermarks github was helpful.
