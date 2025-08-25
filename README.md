### edgeHandler_strategy branch

This is the an alternative version of the [main branch](/../main), in which edge handling is injected into the **ImageProcessing** class and used appropriately just before the image convolution. However, it introduces some overhead and forces to create the extended image each time, rather than once.
