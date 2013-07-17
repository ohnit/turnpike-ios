.. _installation:

############
Installation
############

-----------------------------------------------------------------------

.. _i-without-pods:

******************************
Installation Without CocoaPods
******************************

Download Turnpike Binary and Headers
------------------------------------

Download and extract the Turnpike library binary and header files.

Load the binary and headers into your app
-----------------------------------------

Drag and drop the libTurnpike.a and Headers folder into your Xcode projects. When it prompts you for choosing actions upon import, make sure that "Copy items into destination group's folder (if needed)" is checked and that the binary is being added to your build target.

Import Turnpike
---------------

You can now use Turnpike in your app by importing it:

.. code-block:: objc

    #import "Turnpike.h"


-----------------------------------------------------------------------    

.. _i-with-pods:

****************************
Installation Using CocoaPods
****************************

.. note:: 
   (Pod info coming soon, pending Pod submission)

Install CocoaPods
-----------------

Open up your Terminal and type:

.. code-block:: objc

    $ gem install cocoapods
    $ pod setup

Now CocoaPods should be installed.

Create a Podfile
----------------

If you don't already have one, you'll need a Podfile to specify and install CocoaPods libraries in your app. 

In your Xcode project directory, create a text file named ``Podfile``  with the contents:

.. code-block:: objc

    platform :ios
    pod 'Turnpike', '~>1.0'

Install the pod
---------------

To install your pods, run from the Terminal:

.. code-block:: objc

    $ pod install

Open your workspace
-------------------

Your project using CocoaPods must be opened as a workspace, not a project. To do this explicity from the Terminal, run:

.. code-block:: objc

    $ open MyAwesomeApp.xcworkspace

Import Turnpike
---------------

You can now use Turnpike in your app by importing it:

.. code-block:: objc

    #import <Turnpike.h>
