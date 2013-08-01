//
//  TPRouter.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPRouter.h"
#import "TPFilterChain.h"
#import "TPHelper.h"
#import "TPParsingHelper.h"
#import "TPURIHelper.h"

@interface TPRouter()

/**
 The filters to be added to and processed a filter chain.
 */
@property (strong, nonatomic) NSArray *filters;

/**
 The routing callback to invoke when no route is matched upon invocation.
 */
@property (strong, nonatomic) TPRouteDestination defaultRoute;

/**
 Routes that the user has defined
 */
@property (strong, nonatomic) NSDictionary *definedRoutes;

- (void)invokeRoute:(NSString *)route WithSchema:(NSString *)schema AndQueryParameters:(NSDictionary *)queryParameters;
+ (NSDictionary *)matchIncomingRouteSegments:(NSArray *)incomingRouteSegments ToDefinedRouteSegments:(NSArray *)definedRouteSegments;

@end

@implementation TPRouter

#pragma mark Private Interface

@synthesize filters=_filters;
- (NSArray *)filters {
    if (!_filters){
        _filters = [NSArray array];
    }
    return _filters;
}

@synthesize defaultRoute=_defaultRoute;
- (TPRouteDestination)defaultRoute {
    if (!_defaultRoute) {
        _defaultRoute = ^(TPRouteRequest *request) {
            
        };
    }
    return _defaultRoute;
}

@synthesize definedRoutes=_definedRoutes;
- (NSDictionary *)definedRoutes {
    if (!_definedRoutes) {
        _definedRoutes = [NSDictionary dictionary];
    }
    return _definedRoutes;
}

- (void)invokeRoute:(NSString *)route WithSchema:(NSString *)schema AndQueryParameters:(NSDictionary *)queryParameters {
    // The route request which will get set either when the incoming route is matched to a defined route, or when it is not matched and falls back on the default route
    TPRouteRequest *request = nil;
    // The callback to execute at the end of the filter chain. Either this will be the value of the key / value pair of the route it matches, or it will be whatever the default route is defined to be (it does nothing, by default)
    TPRouteDestination callback = nil;
    // The route being invoked in route segments, from the route split by '/'s
    NSArray *routeSegments = [route componentsSeparatedByString:@"/"];
    // Start checking through each of our defined routes
    for (NSString *definedRoute in self.definedRoutes.allKeys) {
        // The defined route in route segments, from the route split by '/'s
        NSArray *definedRouteSegments = [definedRoute componentsSeparatedByString:@"/"];
        // If the routes have a different number of segments, they definitely are not the same route
        if (routeSegments.count != definedRouteSegments.count) continue;
        // Let try and match our incoming route to this defined route. If successful, we'll get a dictionary back (empty if the route has no variables), but if the match is unsuccessful, we'll get nil back
        NSDictionary *routeParameters = [[self class] matchIncomingRouteSegments:routeSegments ToDefinedRouteSegments:definedRouteSegments];
        // If the incoming route and the defined route do not match, continue
        if (routeParameters == nil) continue;
        // Now that we are sure we have a match and we have route parameters, we can build a request
        request = [TPRouteRequest routeRequestWithURLSchema:schema QueryParameters:queryParameters MatchedRoute:definedRoute AndRouteParameters:routeParameters];
        // Also, we can set our callback because we have found the appropriate key for our dictionary
        callback = [self.definedRoutes objectForKey:definedRoute];
        // We've found our match, so lets break out of this for loop
        break;
    }
    // If by this point we still haven't set our request (meaning the incoming route did not match any of the defined routes), we will fallback on the default route
    if (!request || !callback) {
        // Create a route request with the schema and query string parameters, but with a nil route and route parameters
        request = [TPRouteRequest routeRequestWithURLSchema:schema QueryParameters:queryParameters MatchedRoute:nil AndRouteParameters:nil];
        // Set the callback to the default route callback
        callback = self.defaultRoute;
    }
    // Now we have a request and callback, so we can start our filter chain
    [TPFilterChain dofilterChainWithFilters:self.filters Request:request AndCallback:callback];
}

+ (NSDictionary *)matchIncomingRouteSegments:(NSArray *)incomingRouteSegments ToDefinedRouteSegments:(NSArray *)definedRouteSegments {
    // Create a temporary mutable dictionary to add route parameters to
    NSMutableDictionary *routeParameters = [NSMutableDictionary dictionary];
    // Go through each route segment of the defined route, if corresponding incoming route segment is not a variable and does not exist or is not equal to the route parameters, then return nil as this is not a match
    for (int i = 0; i < definedRouteSegments.count; i++) {
        // Get the defined route segment that we're currently comparing
        NSString *definedRouteSegment = [definedRouteSegments objectAtIndex:i];
        // Get the corresponding incoming route segment
        NSString *incomingRouteSegment = [incomingRouteSegments objectAtIndex:i];
        // Check to see if this is a route parameter (variable in route)
        if ([definedRouteSegment characterAtIndex:0] == ':') {
            // Set the route parameter
            [routeParameters setValue:incomingRouteSegment forKey:[definedRouteSegment substringFromIndex:1]];
            // Continue our loop
            continue;
        }
        // If it's not a variable, lets make sure it matches our defined route segment
        else if ([definedRouteSegment isEqualToString:incomingRouteSegment]) {
            // It does match, so lets continue
            continue;
        }
        // Our incoming and defined route segments do not match, return nil
        else {
            return nil;
        }
    }
    // We've matched both routes and parsed the route parameters, so lets return the route parameters to indicate that we're done (an immutable copy, of course)
    return [routeParameters copy];
}

#pragma mark Factory Method

+ (TPRouter *)router {
    TPRouter *router = [[TPRouter alloc] init];
    return router;
}

#pragma mark Mapping Routes

- (void)mapRoute:(NSString *)format ToDestination:(TPRouteDestination)destination {
    // Create a temp mutable copy to add a key to
    NSMutableDictionary *tempDefinedRoutes = [self.definedRoutes mutableCopy];
    // Add a key to the mutable copy
    [tempDefinedRoutes setValue:destination forKey:format];
    // Set the defined routes to an immutable copy of our temp mutable copy
    self.definedRoutes = [tempDefinedRoutes copy];
}

- (void)mapDefaultToDestination:(TPRouteDestination)destination {
    // Set the default route
    self.defaultRoute = destination;
}

#pragma mark Filter Chains

- (void)appendFilter:(id <TPFilterProtocol>)filter {
    // Set the filters array to the filters array + the new filter
    self.filters = [self.filters arrayByAddingObject:filter];
}

- (void)appendAnonymousFilter:(TPFilterBlock)filterBlock {
    // Create the anonymous filter from the filter block
    TPAnonymousFilter *filter = [TPAnonymousFilter filterWithBlock:filterBlock];
    // Set the filters array to the filters array + the new filter
    self.filters = [self.filters arrayByAddingObject:filter];
}

#pragma mark Invoking URLs & Routes

- (void)resolveURL:(NSURL *)url {
    NSURL *sanitizedURL = [TPURIHelper sanitizeURL:url];
    NSString *scheme = [TPURIHelper safeSchemeFromURL:url];
    NSString *sanitizedPath = sanitizedURL.path ? sanitizedURL.path : @"";
    NSString *rawRoute = sanitizedURL.host ? ([sanitizedURL.host stringByAppendingString:sanitizedPath]) : sanitizedPath;
    [self invokeRoute:[TPParsingHelper sanitizeMappedPath:rawRoute] WithSchema:scheme AndQueryParameters:[TPURIHelper queryStringToMap:sanitizedURL.query]];
}

@end
