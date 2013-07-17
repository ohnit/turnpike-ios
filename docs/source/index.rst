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

.. rubric:: Developed by URX__ to enable mobile deeplinking in apps.

.. __: http://www.urx.com/

.. _o-main-functions:

Turnpike's Core Responsibilities
--------------------------------

- :doc:`mapping-routes` either defined routes or a fallback ``404`` route
- :doc:`route-requests` provide a consistent representation for processing incoming routes and URLs
- :doc:`filter-chains` to decouple request processing from routing logic

Brief Example
=============

Defining routes:

.. code-block:: objc

	TPRouter *router = [TPRouter router];
	    [router mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
	        [Products displayProductWithId:[request.routeParameters valueForKey:@"product_id"]];
	}];

Adding filters:

.. code-block:: objc

	[router addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
	     NSLog(@"%@",request.matchedRoute);
	     [filterChain.doFilterWithRequest:request];
	}];

Resolving route requests:

.. code-block:: objc

	[router invokeInternalRoute:@"product/1988"]

Turnpike Request Lifecycle
==========================

When resolving deeplink URI's, Turnpike takes the following steps:

- Searches for a matching defined route, and falls back on the default route if no match is found.
- Applies each filter in the filter chain to the created ``TPRouteRequest`` object.
- Invokes the callback associated with the matched (or default) route.

