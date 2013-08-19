

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