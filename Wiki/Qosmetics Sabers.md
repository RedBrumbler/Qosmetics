# Qosmetics Saber Exporter

![QosmeticsGif](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Generic/Qosmetics.gif)

If you haven't already, check out the [General Setup Guide](https://github.com/RedBrumbler/Qosmetics/wiki/Qosmetics-General-Setup)

Now that you've setup your qosmetics project, let's get to exporting a saber! This guide will not cover making an actual model, there are enough resources available online on how to make models with for example blender.

# Exporter Rundown

You'll find the QuestSaber exporter in `Tools->Qosmetics->Exporters->QuestSaber`:

![QuestSaberLocation](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/QuestSaberLocation.png)

![QuestSaberWindow](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/QuestSaberWindow.png)

After opening the exporter if it's not already opened, you'll find the options to fill out. (if you don't see any options, click the triangles)

### Top line

At the top you will find the name of the exporter (QuestSaber), and 3 buttons, the first one will load the scene with name questsabers from Assets/Scenes, just like the scene changer mentioned in the [Generic guide]().

The second button will spawn in a new saber template so that all neccesary data is already added to it. (Adding scripts to an object is not really supported due to some issues I was having with that) This is the intended way of making more than 1 saber in the same scene!

The last button will export all currently active saberss to a directory you select, this is ideal for making lots of models in one go! I recommend to enable the set layer mask option in the exporter config menu, as this will make the images correct.

### BMBF mod info

The BMBF mod Info is what you will want to fill out with your own info. The Advanced bmbf mod info has some extra information you *can* fill out, but this is not neccesary to make a functioning saber and thus I will not go into more detail on this.

### Saber Config

Next you will find the saber config list, these are options that can be changed on your saber to add (or remove) behaviour from the saber.
 - **Enable base game Fake Glow** will toggle to enable the fake glow that's on the default sabers if you want it to, most of the time this can be left off though
 - **Has custom Trails** This is a new feature for sabers, and will allow you to add custom trails to the sabers, and define your own materials! enabling this on a saber will make the project add some Trail objects to your saber automatically (if you don't want them just disable the option and delete the Trail objects in your LeftSaber and RightSaber), this looks like this in the object hierarchy:

![CustomTrailHierarchy](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/CustomTrailHierarchy.png)

**Make sure to have the Start and End transforms named the same as in the example! also make sure they are a child of the Trail object!**

# Custom Saber Export
However, before we take a look at how to configure a custom trail, let's first find out how to make a saber work with the exporter. As seen in the previvous image which was used to illustrate the trail hierarchy, there's a `LeftSaber` and `RightSaber` GameObject. This is where we want to add our models to. The models can be added by first putting the model of your choice in the project files down below, and after that you can simply drag them onto `LeftSaber` or `Rightsaber` and you will find them pop up in the central view!

![ModelInView](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/ModelInView.png)

Now that they are in your view, you need to make sure that the saber lines up with the provided template. This template will have a dark gray and a colored side, the dark gray side is about where the player will hold the saber, and the size of the template is also accurate to what is in beat saber. So try to keep your sabers the same size as the given template! As you can see, I need to move this saber down a bit, and when I do it looks a bit like this:

![LinedUp](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/LinedUp.png)

Now that we know that it's lined up, we can disable the template object, or just completely delete it as it's not needed, now we want to apply the materials to the saber, this is as simple as dragging the materials onto the object in the middle view:

![Materials](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/Materials.png)

Now that you have materials on the saber, you probably also want some custom colors (or not) on some materials. Qosmetics will, by default, not put custom colors on your materials, you have to do something to them to get custom colors on your materials. To make the mod put a custom Color on your Color variable you need to either toggle a Custom Colors tickbox or have a Glow value higher than 0. The following material does not get custom materials, even though the Glow value is higher than 0, this is because the Custom Colors tickbox takes priority over the Glow value.

![CustomColors](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/CustomColors.png)

now that you know how to put in the model and put materials on the model, you can repeat the same process for the other saber. Make sure that materials that need custom colors are specific to one saber type! putting the same material on 2 sabers will apply the color of one saber to both sabers.

Next is the exporting of the qsaber, this is as simple as having the info filled out, the config set, and pressing a button! The project will automatically generate the bmbfmod.json with your info, it will generate the cover image, and put it all in a zip for you, and proceed to open the export location after you press ok on the dialog box!

That's it! that's how to make a saber with the qosmetics exporter!

### Custom Trails
Now we will have a look at how to configure a trail:

![TrailReferences](https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/Wiki/Images/Sabers/TrailReferences.png)

As you can see, there are some options here, let's go through them one by one:
 - **Trail Material** This is where you drag in the material to use for the trail
 - **Color Type** this drop down will show LeftSaber, RightSaber and Custom. If configured with Leftsaber this trail will use the leftsaber color, RightSaber gives rightsaber color, and custom color will use the color that's configured up next:"
 - **Trail Color** the color used when colortype is defined as Custom
 - **Multiplier Color** the color the trail color will be multiplied by
 - **Length** the length of the trail, 14 is a good default size if you don't know how long you want the trail to be
 - **Whitestep** this is the "motion blur" people complain about so often, I prefer to leave it at 0

To make the game start the trail at the place you want, make sure that the object with the TrailReferences has 2 children called `TrailStart` and `TrailEnd`!

### Special Shader Properties

This section is for the people who make shaders, or who want to edit shaders, and will tell you the special Property names used within the Qosmetics mod:
 - **_Color** is the keyword used to put the CustomColor into, if the custom color is allowed
 - **_OtherColor** if custom colors are enabled, this color gets set to the *other* saber color, have fun with this :)
 - **_CustomColors** normally paired with a \[MaterialToggle\] in front of it, this property name has the highest priority when it comes to allowing custom colors
 - **_Glow** in absence of a _CustomColors property this property will be used to determine wether or not to use custom colors, if it's above 0 the material gets custom colors
 - **_Bloom** Lowest priority property to decide wether or not to use custom colors

Next are some examples of how these properties can be added into your properties block:
```
Properties
{
    _Color ("Color", Color) = (1,1,1,1)
    _OtherColor ("Other Color", Color) = (1,1,1,1)
    [MaterialToggle] _CustomColors("Custom Colors", Float) = 0
    _Glow ("Glow", Range (0, 1)) = 0
    _Bloom ("Bloom", Range (0, 1)) = 0
}
```
