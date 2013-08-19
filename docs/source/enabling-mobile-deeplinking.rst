

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