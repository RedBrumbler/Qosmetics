# Qosmetics Wall Exporter

![QosmeticsGif](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Generic/Qosmetics.gif)

If you haven't already, check out the General Qosmetics exporter setup guide [here](TODO URL)

Now that you've setup your qosmetics project, let's get to exporting a Wall! This guide will not cover making materials in detail, though it will give some pointers where neccesary, there are a lot of resources available online to learn how to make shaders and materials already, I will however touch on specific things that have to be done with walls.

# Exporter Rundown

You'll find the QuestWall exporter in `Tools->Qosmetics->Exporters->QuestWall`:

![QuestWallLocation](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/QuestWallLocation.png)

![QuestWallWindow](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/QuestWallWindow.png)

After opening the exporter if it's not already opened, you'll find the options to fill out. (if you don't see any options, click the triangles)

### Top line

At the top you will find the name of the exporter (QuestWall), and 3 buttons, the first one will load the scene with name questwalls from Assets/Scenes, just like the scene changer mentioned in the [Generic guide]().

The second button will spawn in a new wall template so that all neccesary data is already added to it. (Adding scripts to an object is not really supported due to some issues I was having with that) This is the intended way of making more than 1 wall in the same scene!

The last button will export all currently active walls to a directory you select, this is ideal for making lots of models in one go! I recommend to enable the set layer mask option in the exporter config menu, as this will make the images correct.

### BMBF mod info

The BMBF mod Info is what you will want to fill out with your own info. The Advanced bmbf mod info has some extra information you *can* fill out, but this is not neccesary to make a functioning wall and thus I will not go into more detail on this.

### Wall Config

Next you will find the wall config list, these are options that can be changed on your wall to add (or remove) behaviour from the wall.
 - **Replace Core Material** wether or not to replace the core material
 - **Replace Core Mesh** wether or not to replace the core mesh (3d model)
 - **Disable Frame** wether or not to disable the frame
 - **Disable Fake Glow** wether or not to disable the Fake Glow on the frame
 - **Replace Frame Material** wether or not to replace the frame material
 - **Replace Frame Mesh** wether or not to replace the frame mesh (3d model)
 - **Score Submission Disabled** wether or not to disable score submission if the player has this wall active
 Make sure to toggle this last thing if the wall is cheaty, as a rule of thumb I always say that if a wall gives the player better visibility than the default walls, alpha < 0.5 generally, **disable the scores**.

# Custom Wall Export

When it comes to walls, there's less to explain about the export process. When you add a new template with the button mentioned earlier in [Top line](), all you need to do is have the things you specify in the config be on the Core and Frame Objects in the wall object:

![WallHierarchy](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/WallHierarchy.png)

If you want to replace the core materials, make sure there is at least 1 material on the mesh renderer on core, if you want to replace the core mesh, make sure there is a mesh in the core mesh filter:

![CoreObject](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/CoreObject.png)

The same goes for the frame materials, and the mesh, make sure they are on the object if you want to replace them:

![FrameObject](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/FrameObject.png)

*To select the objects and have their information show up on the right, just click on them in the hierarchy on the left**

After applying the materials, meshes and config, you can export by clicking the export button in the wall exporter! that's it! that's how to export a custom wall! you can export the wall by itself, but you can also have the project export a full bmbf zip for you, including the cover image and the bmbfmod.json.


### Custom Shaders/Materials
For the Wall Core, any material can be used really. The real issue is with the frame. if you resize the WallTemplate object, you will notice that the frame stays the same width if the material that came with the project is applied to it. Try adding a random material to it, and you will see that it will not work like this anymore! this is a clever trick beatgames pulled, they change the way the mesh is rendered based on the size of the object, and do this with a special shader. Luckily for us Ikeiwa was so kind as to help me (read: Ikeiwa did all the work) make some methods to apply the same transforms to the mesh, and render a nice frame. all you need to do is put these things in your shaders:

In the properties block at the top, you will need to paste in these 2 properties:
```
_EdgeSize("Edge Multiplier", float) = 1
_SizeParams("Size Params", Vector) = (0.5,0.5,0.5,0.05)
```

Below the mention of `#include "UnityCG.cginc"` you will need to add this line:
```
#include "Assets/Qosmetics Exporters/CGINC files/frame.cginc"
```

Then right abovve the v2f vert function you will need to add these to make sure the program knows about our earlier defined properties:
```
float4 _SizeParams;
float _EdgeSize;
```

Lastly, in the v2f vert function, we want to edit the vertices, and we can do this by doing this:
```
v.vertex.xyz += frameDisplacement(v.vertex, 0.99, _SizeParams, _EdgeSize);
```

For a practical example just open up the file at `Assets/WALL/Shaders/sh_wall_frame.shader`

![WallFrameShader](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Walls/WallFrameShader.png)

For materials, the same rules apply when it comes to custom colors as with sabers or bloqs, any material with a Custom Color toggle can get custom materials on its Color variable, but only if it's enabled, if a Custom Color toggle doesn't exist a Glow value can be read, which then needs to be higher than 0.

### Special Shader Properties
This section is for the people who make shaders, or who want to edit shaders, and will tell you the special Property names used within the Qosmetics mod:
 - **_Color** is the keyword used to put the CustomColor into, if the custom color is allowed
 - **_CustomColors** normally paired with a \[MaterialToggle\] in front of it, this property name has the highest priority when it comes to allowing custom colors
 - **_Glow** in absence of a _CustomColors property this property will be used to determine wether or not to use custom colors, if it's above 0 the material gets custom colors
 - **_Bloom** Lowest priority property to decide wether or not to use custom colors

Next are some examples of how these properties can be added into your properties block:
```
Properties
{
    _Color ("Color", Color) = (1,1,1,1)
    [MaterialToggle] _CustomColors("Custom Colors", Float) = 0
    _Glow ("Glow", Range (0, 1)) = 0
    _Bloom ("Bloom", Range (0, 1)) = 0
}
```