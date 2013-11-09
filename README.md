PebblePieces
============

Reusable UI Components for Pebble Smartwatch Apps. I'll be using the PP* prefix to avoid namespacing conflicts a la Apple. Our cast of characters currently includes:

##PPToaster
Animated, self-dismissing modal notifications with minimal setup. This is a "singleton", e.g. a global instance is created statically. It binds automatically to the current window so no setup is needed, simply call:

``pptoaster_pop("Loading complete", 2000, PPToasterAppearFromBottom);``

This will pop a toaster that hangs around for 2 seconds from the bottom of the screen. Toasters can also come from the top or middle. Fonts are customizable. Not much else is - you're currently limited to one line of text so keep your messages short.

##PPSpinner
An indefinite progress indicator. Was going to be one of those spinny things, but it ended up being more like those marching bars, you know the kind: ![loader](http://f.cl.ly/items/0v3M263E3E1O2h3h292j/threebars.gif)

You can customize the number of bars, corner radius, animation speed, and of course size. Since this one might have multiple instances, it is a ``Layer`` "subclass", e.g. a ``Layer`` with private data attached, typedef'd to a ``PPSpinnerLayer``. Use the create/destroy methods to manage their lifecycles:

```c
PPSpinnerLayer *my_spinner = ppspinner_create(…);
ppspinner_start(my_spinner);
ppspinner_stop(my_spinner);
ppspinner_destroy(my_spinner);
```

##Contributions
That's all for now. I have a console window that I may add in. If you have your own reusable components, please consider contributing them!

