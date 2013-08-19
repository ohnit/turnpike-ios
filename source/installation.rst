

##############################
Installation
##############################

This installation tutorial will show you how to add URX Turnpike to your existing
project, and how to keep it up-to-date.

Prerequisite
------------

With Turnpike, we'll be using `Git <http://git-scm.com/%20Git>`__ to
manage versions. If you haven't used Git before, I recommend running
through `Try Git <http://try.github.com/%20Try%20Git>`__, a quick Git
tutorial. You must have Git installed to complete this tutorial.

Step 1: Add your Project to Workspace
-------------------------------------

**Note: You may skip to step two if you are already developing your App
in a workspace.**

Starting with Xcode 4, Apple introduced the concept of
`Workspace <http://developer.apple.com/library/ios/#featuredarticles/XcodeConcepts/Concept-Workspace.html%20Apple%20Workspace%20Notes>`__
management. This took the prior concept of Project Management, and
allowed encapsulated projects, and shared documents, to exist in a space
together so you can work on, and reference them, simultaneously.
Projects in a Workspace also share a build directory, making it easy to
link to binaries of projects in the workspace.

To start, create a folder to house our workspace. Because my app is
called ``TestApp``, I will create a folder called ``TestAppWorkspace``,
where all my dependencies will live. However if you have a single code
base for multiple apps, you may want to have all of your apps in the
same Workspace. I only have one, so this will be ``TestAppWorkspace``.

.. figure:: source/images/installation/1.png
   :alt: Create Workspace folder

   Create Workspace folder
In Xcode, Go to File->New->Workspace, or press Control + Command + N, to
create a new Workspace, and save it in the folder we just created with
the name of your Workspace.

.. figure:: source/images/installation/2.png
   :alt: Create Workspace

   Create Workspace
Now that you have a Workspace, we need to put some projects into it!
Lets start with your existing project.

Before we add it to the Workspace in Xcode, let's move the project
folder into the Workspace folder, just to keep the organization cleaner
and not have your Workspace's managed projects all over the place

.. figure:: source/images/installation/3.png
   :alt: Move project to workspace folder

   Move project to workspace folder
Go to File->New->Add Files to "TestAppWorkspace" (where
"TestAppWorkspace" is your workspace), or press Option + Command + A,
and you'll be able to select files. Navigate to your App's
``.xcodeproj`` file and select that. Make sure "Copy items into
destination group's folder (if needed)" is checked, and click add.

.. figure:: source/images/installation/4.png
   :alt: Add project to Workspace in Xcode

   Add project to Workspace in Xcode
Step 2: Add Turnpike to Workspace
---------------------------------

Now that you have your project in a workspace, open your Terminal and
navigate to your Workspace folder. Once there, run
``git clone https://github.com/URXtech/turnpike-ios.git`` and you should
have the latest stable version of URX Turnpike (for iOS) in your
Workspace Folder.

**Note: If you have already cloned a version of Turnpike, skip to the
last step on maintenance to update it before continuing.**

.. figure:: source/images/installation/5.png
   :alt: Add project to Workspace in Xcode

   Clone Turnpike
Next we need to add the Turnpike project as we did for our own App, so
in Xcode, go to File->New->Add Files to "TestAppWorkspace" (where
"TestAppWorkspace" is your workspace), or press Option + Command + A,
and this time select ``Turnpike.xcodeproj`` in the ``turnpike-ios``
folder.

.. figure:: source/images/installation/6.png
   :alt: Add Turnpike to Workspace

   Add Turnpike to Workspace
Step 3: Test & Compile Turnpike
-------------------------------

First we want to compile Turnpike into a binary. On the top bar of the
editor window, under "Scheme", select "Turnpike", and select a simulator
to run on. Try giving the tests a run (Command + U), and the console
should report that everything ran fine.

Although running the test involved Build Phases, we didn't get a binary
from it, as evidenced by expanding the Products group in the Turnpike
project, and seeing that ``libTurnpike.a`` is red, meaning Xcode can't
find it.

.. figure:: source/images/installation/7.png
   :alt: Test Turnpike

   Test Turnpike
With static libraries, like Turnpike, tests can only be performed on the
simulator, while binaries can only be compiled by targeting actual
device architecture, so select "iOS Device" under Turnpike's schema
("iOS Device" may have the name of your iOS device if it's plugged into
your computer), and build Turnpike (Command + B). ``libTurnpike.a``
should now be black.

Step 4: Linking to Turnpike Binary and Headers
----------------------------------------------

This next part is a little more complicated, but follow along, and it'll
be easy.

Now that it's built we need to tell our own App to use Turnpike. First
lets point to Turnpike's headers so Xcode can know about them and not
get upset when we claim they exist.

In Xcode, click on your project settings, and select your "Project" (not
"Target"). Select the "Build Settings" tab and in the search field,
search for "User Header Search Paths". You should see "User Header
Search Paths" show up. Double click on the rightmost portion of this row
to bring up a box with little + and - buttons. Hit the + button and type
``../turnpike-ios/Turnpike`` (assuming your project is in the Workspace
directory, as it should be if you followed along).

.. figure:: source/images/installation/8.png
   :alt: Link to Turnpike Headers

   Link to Turnpike Headers
Next, in your project settings, select your "Target", and select the
"Build Phases" tab. In your "Link Binary With Libraries" phase, hit the
+ button and select ``libTurnpike.a`` from the list (under Workspace).

.. figure:: source/images/installation/9.png
   :alt: Link to static library

   Link to static library
Step 5: Adding a Custom URL Scheme
----------------------------------

If your app doesn't yet have a custom URL scheme you'll need to have
your app register for one on the user's device. Fortunately, this is now
easier than ever in Xcode. In your app's project settings, select your
"Target", and click on the info tab. There, you should expand the "URL
Types" section. If you already have a custom URL registered, you'll see
it there, and if not, this will be empty and say "No URL Types".

Click the + button to create a new URL Type. Most of this is not
important to us, as it deals with file handling. What is important is
the Identifier and URL Schemes fields.

For both these fields, you want to enter your App's Bundle Identifier in
`reverse domain name
style <http://en.wikipedia.org/wiki/Reverse_domain_name_notation%20Reverse%20Domain%20Name%20Notation>`__.
This is recommended practice by Apple (although even they are guilty of
not doing this in their sample app) as behavior for collisions is
undefined. You can find your bundle identifier by switching to the
"Summary" tab in your "Target".

**Note: Even if you already have a URL schema registered, we recommend
you register one in the reverse domain name style and use that one for
creating deep links. This will prevent any non-malicious collisions, and
is what we recommend as best practice.**

.. figure:: source/images/installation/10.png
   :alt: Create URL Schema

   Create URL Schema
Step 6: Setting up Turnpike in your App
---------------------------------------

The hard part is all finished, now you just need to tell your app what
routes you want to register and when to invoke a route.

Open up your ``AppDelegate.m`` file (in my case ``TAAppDelegate.m``),
and under the first ``#import`` statement, add
``#import <Turnpike/Turnpike.h>``.

In your
``- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions``
method, you will register routes and filters. For the sake of this
tutorial we won't get into Filters, but they are powerful ways to put
logic on top of your routing. In this method we'll add a test route
which will print "Hello World!" to the console. Add:

::

    // Override point for customization after application launch.
    [Turnpike mapRoute:@"hello" ToDestination:^(TPRouteRequest *request) {
        NSLog(@"Hello World!");
    }];

The last thing we have to do is let Turnpike pick up the incoming URL.
To do this, we'll need our AppDelegate to implement
``- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation``
(and
``- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url``
if you're targeting iOS < 4.2).

**Note:
``- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation``
will be called instead of
``- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url``
on iOS >= 4.2 devices. If you want to support older devices, implement
both.**

In your ``openURL`` method, all you need to add is
``[Turnpike resolveURL:url];`` and return ``YES``.

.. figure:: source/images/installation/11.png
   :alt: Setting up your App Delegate

   Setting up your App Delegate
Congratulations, you have now added Deeplinking to your app!

Step 7: Feel the Magic (Make sure it works)
-------------------------------------------

Lets do a quick test to make sure it works. Build and Run your app on
the simulator or your device. Go to Safari and enter your URL Schema +
":hello" and hit "Go". For my app this is "``com.urx.TestApp:hello``\ ".

.. figure:: source/images/installation/12.png
   :alt: Deeplinking from Mobile Safari

   Deeplinking from Mobile Safari
**Note: this could also be com.urx.TestApp://hello,
com.urx.TestApp:///hello or just com.urx.TestApp:hello. Leading
slashes after the colon are ignored, and the route in this case, is just
hello.**

This should quickly switch to your app and in your Xcode console, you
should see hello world. Congratulations, you've integrated URX Turnpike!
Time for cookies!

.. figure:: source/images/installation/13.png
   :alt: Hello World

   Hello World
Bonus Step: Keeping Up-To-Date with URX Turnpike
------------------------------------------------

Updating Turnpike is easy! Just open up your console, navigate to your
Turnpike directory, then run ``git pull origin master`` and the newest
version will update straight from GitHub.

.. figure:: source/images/installation/14.png
   :alt: Updating From GitHub

   Updating from GitHub
After updating, make sure to re-build your Turnpike binary in Xcode.
Select Turnpike from the scheme list, and make sure you're building for
"iOS Device" (or the name of the device you have plugged into your
computer). Press Command + B to build, and you're all updated!
