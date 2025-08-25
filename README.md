# kip-sequential - edgeHandler_strategy branch

## Differences

This is the an alternative version of the [main branch](https://github.com/marcopaglio/kip-sequential "Repository of kip-sequential's main branch") in which edge handling is injected into the **ImageProcessing** class and used appropriately just before the image convolution. However, it introduces some overhead and forces to create the extended image each time, rather than once.
