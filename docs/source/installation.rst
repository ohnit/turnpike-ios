.. _installation:

############
Installation
############

With CocoaPods
==============
(Coming Soon)

Step 1: Install CocoaPods (if already installed, skip)
------------------------------------------------------

Open up your Terminal and type:

    $ gem install cocoapods
    $ pod setup

Now CocoaPods should be installed.

Step 2: Create a Podfile for your project
-----------------------------------------

If you don't already have one, you'll need a Podfile to specify and install CocoaPods libraries in your app. In your Xcode project directory, create a text file named ``Podfile``  with the contents:

    platform :ios
    pod 'Turnpike', '~>1.0'

Step 3: Install the pod
-----------------------

To install your pods, run from the Terminal:

    $ pod install

Step 4: Open your workspace
---------------------------

Your project using CocoaPods must be opened as a workspace, not a project. To do this explicity from the Terminal, run:

    $ open MyAwesomeApp.xcworkspace

Step 5: Import Turnpike
-----------------------

You can now use Turnpike in your app by importing it:

    #import <Turnpike.h>


Without CocoaPods
=================

Step 1: Download Turnpike Binary and Headers
--------------------------------------------

Download and extract the Turnpike library binary and header files.

Step 2: Import the binary and headers into your app
---------------------------------------------------

Drag and drop the libTurnpike.a and Headers folder into your Xcode projects. When it prompts you for choosing actions upon import, make sure that "Copy items into destination group's folder (if needed)" is checked and that the binary is being added to your build target.

Step 3: Import Turnpike
-----------------------

You can now use Turnpike in your app by importing it:

    #import "Turnpike.h"