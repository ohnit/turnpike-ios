.. title:: Mobile Deeplinks with Turnpike - Getting Started


.. raw:: html

   <div style="display:none;">

.. toctree::
   :maxdepth: 1

   self
   routing
   filters
   requests
   resolution
      

.. raw:: html

   </div>


########
Turnpike
########

.. highlight:: text

.. rubric:: Developed by URX__ to enable mobile deeplinking in apps.

.. __: http://www.urx.com/

.. _o-main-functions:

Core Responsibilities
---------------------

Turnpike has four main functions:

- **Mapping Routes**: To map callbacks to routes (either a defined route or a fallback "404" route).
- **Filter Chains**: To hold a list of filters to perform as a filter chain on invoked routes.
- **Invoking URLs & Routes**: To processes incoming routes and URLs with the router's defined routes and filter chains.
- **Launching External Apps**: Helper methods to launch external apps, enabling the web of mobile apps.
 
To resolve a deeplink URI, Turnpike does the following:

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


