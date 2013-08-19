.. title:: Mobile Deeplinks with Turnpike - Getting Started


.. raw:: html

   <div style="display:none;">

.. toctree::
   :maxdepth: 1

   self
   installation
   enabling-mobile-deeplinking
   mapping-routes
   route-requests
   filter-chains
      

.. raw:: html

   </div>


###############
Getting Started
###############

.. highlight:: text

.. rubric:: Turnpike was developed by URX__ to enable mobile deeplinking in apps.

.. __: http://www.urx.com/

.. _gs-core-responsibilities:

********************************
Turnpike's Core Responsibilities
********************************

- :doc:`mapping-routes` either defined routes or a default / ``404 not found`` route
- :doc:`route-requests` provide a consistent representation for processing incoming routes and URLs
- :doc:`filter-chains` to decouple request processing from routing logic

********************************
Mobile Deeplinking with Turnpike
********************************

Turnpike lets you map deeplink URI routes to in-app actions.

.. code-block:: objc

  [Turnpike mapRoute:@"product/:product_id" ToDestination:^(TPRouteRequest *request) {
      [Products displayProductWithId:[request.routeParameters valueForKey:@"product_id"]];
	}];

Turnpike allows the creation of filters to centralize the processing logic for incoming requests.

.. code-block:: objc

	[Turnpike appendAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
	     NSLog(@"%@",request.matchedRoute);
	     [filterChain.doFilterWithRequest:request];
	}];

After filter processing, Turnpike resolves route requests to their final in-app destinations.

.. code-block:: objc

	[Turnpike resolveURL:[NSURL urlWithString:@"product/1988"]];

.. _gs-first-steps:

*************************
First Steps with Turnpike
*************************	

The Turnpike SDK can be obtained via GitHub, and we recommend keeping Turnpike updated via Git.

The next step is to :doc:`enable deeplinking within your app <enabling-mobile-deeplinking>`, and integrate Turnpike at the deeplink entry point. 

Next, feel free to read on below about Turnpike's capabilities!

.. _gs-request-lifecycle:

**************************
Turnpike Request Lifecycle
**************************

When resolving deeplink URI's, Turnpike takes the following steps:

- Searches for a matching defined route, and falls back on the default route if no match is found.
- Applies each filter in the filter chain to the created ``TPRouteRequest`` object.
- Invokes the callback associated with the matched (or default) route.


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


###########################
Enabling Mobile Deeplinking
###########################

Custom URL Schemes
==================

To register a URL schema, add the CFBundleURLTypes key to your app’s Info.plist file. CFBundleURLTypes contains an array of dictionaries, each of which defines a URL scheme the app supports. Each dictionary within the array is a key / value pair of CFBundleURLName (string) and CFBundleURLSchemes (array of strings).

The CFBundleURLName should be, according to Apple, in reverse dns format (``com.myCompany.myProduct``).

The CFBundleURLSchemes is an array of custom URL schemas you want your app to respond to. You can add as many as you like, but you only need one.


Implementing handleOpenURL
==========================

In the your's App Delegate, you must implement the optional App Delegate method that allow your app to perform deeplinking. This method is:

.. code-block:: objc

    - (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation

In your implementation of this method, you should resolve the incoming url your router. If you're using Turnpike's shared router (which is all that's needed in most cases), simply call ``[Turnpike resolveURL:url]``. If you're using multiple routers, you can instead call ``[router resolveURL:url]``. 

##############################
Mapping Routes to Destinations
##############################
 
Turnpike can map routes to :ref:`Destinations <mr-assigning-destinations>`. Destinations represent **where to navigate** and/or **what action to take** upon a user's entry into an app via deeplink URI.

Turnpike supports :ref:`many mapped routes <mr-mapped-routes>` and :ref:`one default route <mr-default-route>`.

.. _mr-default-route:

Default Route
=============

To define a Default Route, use the method ``mapDefaultToDestination``.

The Default Route is invoked when no route is matched. By default, this does nothing and just launches your app. 

.. note:: 
   The Default Route can be used for a default landing page, or to gracefully degrade to a fallback page for incoming URI's that don't match any known paths.
 
.. _mr-mapped-routes:

Mapped Routes
=============
 
Mapped routes are one of: 
- **Static**: ``about/team``
- **Dynamic**: ``user/:user_id/profile/:page_number``

.. note::
   **Route Parameters**  are variables parsed out of URI templates, like ``:user_id`` or ``:page_number``.

.. _mr-assigning-destinations:

Assigning Destinations to Routes 
--------------------------------

For example, if a resolved deeplink should push a Product Page onto our UINavigation Controller, we might use a callback like this:

.. _mr-examples-product-page:

.. code-block:: objc

    [Turnpike mapRoute:@"product/:product_id" ToDestination:^(TPRouteRequest *request) {
        UINavigationController *navigationController = (UINavigationController*)self.window.rootViewController;
        [navigationController popToRootViewControllerAnimated:NO];
     
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
        ProductViewController *productViewController = [storyboard instantiateViewControllerWithIdentifier:@"ProductViewController"];
     
        [navigationController pushViewController:productViewController animated:NO];
    }];
    
.. _using-dynamic-routes:

Using Dynamic Routes
--------------------

In the case of dynamic routes, route parameters can be accessed in the filter or callback through the ``TPRouteRequest``, by calling ``request.routeParameters``. 

.. note::

   Route ``user/:user_id``, when invoked as ``user/32`` would have the route parameter ``:user_id`` made available in the ``TPRouteRequest``. 
   Calling ``[request.routeParameters valueForKey:@"user_id"]`` returns the value of ``user_id`` from the ``NSDictionary routeParameters``. 
   Since all variables are stored as NSSTring, this would return the NSString representation of ``"32"``.

Suppose we want the links ``tab/travel`` and ``tab/food`` to open different UI tabs. Using route param parsing, we can map the route ``tab/:tab_name`` to a callback that uses the ``:tab_name`` value to focus the appropriate UI element.

.. _mr-examples-switch-to-a-tab:

.. code-block:: objc

    self.tabBarController.selectedIndex = 1;
    [Turnpike mapRoute:@"tab/:tab_name" ToDestination:^(TPRouteRequest *request) {
        int tabIndexToSet = [[request.routeParameters valueForKey:@"tab_name"] isEqualToString:@"Home"] ? 0 : 1;
        [[UIApplication sharedApplication].delegate window].rootViewController.tabBarController.selectedIndex = tabIndexToSet;
    }];

##############
Route Requests
##############

Route Requests hold data about the in-app session created as a user enters an app from a deeplink URI. 

Requests have a lifecycle consisting of 3 stages:

- **Creation**: Requests are created by the ``Router`` when resolving a route. See :ref:`below <rr-route-request-creation>` for details.
- **Filtration**: Requests are passed through :doc:`filters <filter-chains>`
- **Resolution**: After filtration, the ``Destination`` callback is invoked to navigate the user to a predefined page or action within the app. 

The ``TPRouteRequest`` contains both external invocation specific information, as well as route specific information. If you are writing a route or filter, it is important to check for ``nil`` fields in the Route Request.

.. _rr-deeplink-metadata:

Deeplink Metadata
=================

The ``urlSchema`` should be one of the custom URL schemas that :doc:`your app has already registered <enabling-mobile-deeplinking>` earlier in the Turnpike documentation. 

.. note::
   
   For more information about iOS deeplink support, please see "`Implementing Custom URL Schemes`_" in Apple's Advanced App Tricks documentation). 

.. _Implementing Custom URL Schemes: http://developer.apple.com/library/ios/documentation/iPhone/Conceptual/iPhoneOSProgrammingGuide/AdvancedAppTricks/AdvancedAppTricks.html#//apple_ref/doc/uid/TP40007072-CH7-SW18

Two of ``TPRouteRequest``'s properties are used pass information specific to deeplink traffic from outside the app: 

- ``urlSchema`` 
- ``queryParameters``

.. code-block:: objc

	if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
	    [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
	}

The ``queryParameters`` are ``NSString`` key/value pairs parsed from the query string (if it has one). If you are inovking a route internally, this value will be nil.

.. _rr-route-request-creation:

Route Request Creation
======================

During Request **Creation**, a ``Router`` attempts to find a matching route for an incoming deeplink URI, ``matchedRoute`` is set to the corresponding defined route (in route format, ie ``user/:user_id``). If no match was found, the default route will be invoked and this will be ``nil``.

After route matching, the ``Router`` parses out route parameters (from variables defined in dynamic routes) and query parameters. Query params and route params are passed into the new Request, along with the matched route and its destination. If the incoming route did not match any defined routes, a ``nil`` route will be passed into the request along with a default Destination.

.. code-block:: objc

	if(request.matchedRoute) {
	    [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
	}

The ``routeParameters`` is an ``NSDictionary`` of route parameters found in the matched route. If no route parameters are found this will be an empty ``NSDictionary``, and if no matched route was found, this will be ``nil``.

#######################
Filters & Filter Chains
#######################

Filters allow you to perform logic with the incoming Route Request before the route's mapped destination is resolved. Filters can be used for authentication, redirecting, analytics, and more. Turnpike's implementation of filters uses a simplified version of the `intercepting filter`_ pattern.

.. _intercepting filter: http://www.oracle.com/technetwork/java/interceptingfilter-142169.html

.. note::
   The goal of routing filters is to reduce code duplication for cross-cutting concerns that should be evaluated on each incoming deeplink (for instance, tracking UTM campaign query parameters on inbound deeplinks).

Filters are processed sequentially in a :ref:`Filter Chain <fc-filter-chain>`. Each ``Router`` has a single ``Filter Chain``; each individual filter runs once on each request processed by the ``Router``. 

Filters can either be added to the filter chain in one of two ways:

- :ref:`Filters <fc-filters>` implementing ``TPFilterProtocol`` can be added using ``appendFilter``. 
- :ref:`Anonymous Filters <fc-anonymous-filters>` can be defined with a block and added using ``appendAnonymousFilter``.

.. _fc-filters:

Filters
=======

.. _fc-implementing-a-filter:

Implementing a Filter
---------------------

Filters themselves are objects that respond to ``TPFilterProtocol``, and as such implement the following method:

.. code-block:: objc

   - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain 

This is the method that is used to process a ``TPRouteRequest``. To pass processing on to the next filter, the active filter should call 

.. code-block:: objc

   [filterChain.doFilterWithRequest:request]

.. _fc-examples-processing-query-params:

Processing Query Parameters with a Filter
-----------------------------------------

.. code-block:: objc

    @implementation MyCouponFilter
    - (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
        if(request.queryParameters && [request.queryParameters valueForKey:@"coupon_id"]) {
            [CouponProcessor validateAndProcessCoupon:[request.queryParameters valueForKey:@"coupon_id"]];
        }
        [filterChain.doFilterWithRequest:request];
    }
    @end


.. _fc-anonymous-filters:

Anonymous Filters
=================

The ``TPAnonymousFilter`` is an object which responds to the ``TPFilterProtocol``, and which lets the user of this object define the behavior of the filter through a block.

``TPAnonymousFilter``'s are useful when making a filter to handle internal logic, where as creating your own class is useful when creating redistributable classes. The main advantage of ``TPAnonymousFilter``'s is being able to avoid the boilerplate required for creating a new class.

.. _fc-implementing-anonymous:

Implementing an Anonymous Filter
--------------------------------

To use the ``TPAnonymousFilter`` you can create a filter object with the factory method ``+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock``.

.. code-block:: objc
    
    [Turnpike addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        if(request.matchedRoute) {
            [MyAwesomeLoggingService logRoute:request.matchedRoute WithParameters:request.routeParameters];
        }
        [filterChain doFilterWithRequest:request];
    }];


.. _fc-filter-chain:

Filter Chain
============

The ``TPFilterChain`` is a queue of filters which get processed as its created. In processing each filter, the filter can decide to continue or abandon the filter chain. Should each filter continue the filter chain until there are no more filters left, the ``TPRoutingCallback`` associated with the route from the ``TPRouteRequest`` is invoked and the chain is complete.

How to inteface with the Filter Chain
-------------------------------------

Filter chains are created by a ``TPRouter`` when a route or URL is invoked. The filters in the filter chain are supplied by the router in the order in which they were added to the router.

.. note::
   The order of execution is defined by the order in which filters are added to the router (the first filter added is the first execute, the last is the last executed). Each filter passes the current request (after processing and/or modifying the request) to the next filter in the filter chain.

The typical way to interface with ``TPFilterChain`` is in your filter's logic when creating a filter. In your filter logic, if you want to continue the filter chain with the current route, you need to call ``[filterChain doFilterWithRequest:request]``. You should not need to call the ``TPFilterChain``'s constructor, unless you are subclassing ``TPRouter`` in some special way.
