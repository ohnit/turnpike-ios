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
