.. _mapping-routes:

###########################
Mapping Routes to Callbacks
###########################
 
Turnpike can map both defined routes and a :ref:`default route <mr-default-route>`.

.. _mr-default-route:

Default Route
=============

The Default Route is invoked when no route is matched. By default, this does nothing and just launches your app.
 
The Default Route is defined with the method ``+ (void)mapDefaultToCallback:(TPRoutingCallback)callback``

.. _mr-defined-route:

Defined Routes
==============
 
Defined routes are written in "route format", where route segments (constants, such as ``login`` or ``user``) and route parameters (variables, such as ``:user_id`` or ``:page_number``) are separated by the route separator, ``/``.
 
Defined routes can either be static, such as ``about/team``, or can dynamic, such as ``user/:user_id/profile/:comment_id``.

.. _mr-assigning-callbacks:

Assigning Callbacks to Routes 
-----------------------------

Defined routes are defined with the method ``+ (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback``.

.. _mr-examples-product-page:

For example, if a resolved deeplink should push a Product Page onto our UINavigation Controller, we might use a callback like this:

.. code-block:: objc

    [Turnpike mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
        UINavigationController *navigationController = (UINavigationController*)self.window.rootViewController;
        [navigationController popToRootViewControllerAnimated:NO];
     
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard" bundle:nil];
        ProductViewController *productViewController = [storyboard instantiateViewControllerWithIdentifier:@"ProductViewController"];
     
        [navigationController pushViewController:productViewController animated:NO];
    }];
    
.. _using-dynamic-routes:

Using Dynamic Routes
--------------------

In the case of dynamic routes, route parameters can be accessed in the filter or callback through the ``TPRouteRequest``, by calling ``request.routeParameters``. The route ``user/:user_id`` invoked as ``user/32`` would have the route parameter ``:user_id`` accessible in the ``TPRouteRequest`` by calling ``[request.routeParameters valueForKey:@"user_id"]`` which gets the value of ``user_id`` from the ``NSDictionary`` ``routeParameters`` (which would return the ``NSString`` of ``32``).

Suppose we want the links ``tab/travel`` and ``tab/food`` to open different UI tabs. Using route param parsing, we can map the route ``tab/:tab_name`` to a callback that uses the ``:tab_name`` value to focus the appropriate UI element.

.. _mr-examples-switch-to-a-tab:

.. code-block:: objc

    self.tabBarController.selectedIndex = 1;
    [Turnpike mapRoute:@"tab/:tab_name" ToCallback:^(TPRouteRequest *request) {
        int tabIndexToSet = [[request.routeParameters valueForKey:@"tab_name"] isEqualToString:@"Home"] ? 0 : 1;
        [[UIApplication sharedApplication].delegate window].rootViewController.tabBarController.selectedIndex = tabIndexToSet;
    }];
 