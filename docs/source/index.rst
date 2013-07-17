.. title:: Mobile Deeplinks with Turnpike - Getting Started


.. raw:: html

   <div style="display:none;">

.. toctree::
   :maxdepth: 1

   self
   mapping-routes
   filter-chains
   requests
   resolving-routes
      

.. raw:: html

   </div>


########
Turnpike
########

.. highlight:: text

.. rubric:: Developed by URX__ to enable mobile deeplinking in apps.

.. __: http://www.urx.com/

.. _o-main-functions:

Turnpike's Core Responsibilities
--------------------------------

- :doc:`mapping-routes` either defined routes or a fallback ``404`` route.
- :doc:`filter-chains` to decouple request processing from routing logic.
- :doc:`resolving-routes` To processes incoming routes and URLs with the router's defined routes and filter chains.
 
Resolving Deeplink URI's
------------------------

- Searches for a matching defined route, and falls back on the default route if no match is found.
- Applies each filter in the filter chain to the created ``TPRouteRequest`` object.
- Invokes the callback associated with the matched (or default) route.

Brief Example
-------------

.. code-block:: objc

	TPRouter *router = [TPRouter router];
	    [router mapRoute:@"product/:product_id" ToCallback:^(TPRouteRequest *request) {
	        [Products displayProductWithId:[request.routeParameters valueForKey:@"product_id"]];
	}];
 
	[router addAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
	     NSLog(@"%@",request.matchedRoute);
	     [filterChain.doFilterWithRequest:request];
	}];
 
	[router invokeInternalRoute:@"product/1988"]


