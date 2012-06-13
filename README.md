ShadowSelf - An interactive visualizer

ShadowSelf is a project I did a while back as an interactive exhibit for an art show. The
high school days. It used openFrameworks 006 and requires a webcam. 

ShadowSelf first snaps an image of the background and then separates the shape of whomever
is in front of the camera. This shape is manipulated and blended into the colorful pattern
that moves with you and constantly cycles.

When no one is in the frame, the canvas is blank and a new set of parameters is generated.
Stepping out of the frame and in again will produce a different pattern and mix of colors.

The big idea was to be inconspicuous until someone passed by, at which point they would 
step backward to see what happened and realize that the visuals were driven by their 
movement. It took people a little while to figure out exactly what they were affecting,
but eventually they saw their own waving arms and dance moves replicated a thousand times
in the spinning patterns. 

A very rudimentary playlist system is also implemented. It is stored in 
bin/data/playlist.xml

The parameters for the shape generation are stored in another XML file at
bin/data/params.xml

Most parameters are self-explanatory and the code is commented. There's not a hell of a
lot to it.

Anyway - Compile it against Openframeworks 006 or 0062 and you can see it in action. 
Take note of the small tweaks made to the OpenCV libraries. 

A sample video can be fount at https://vimeo.com/15919715

Perhaps it will be fun to some of you -- which is pretty much all I can hope for. It's
from my earlier coding days. It's not perfect, but I think it's adequately structured.