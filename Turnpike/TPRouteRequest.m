//
//  TPRouteRequest.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPRouteRequest.h"

@implementation TPRouteRequest

+(TPRouteRequest *)routeRequestWithURLSchema:(NSString *)urlSchema QueryParameters:(NSDictionary *)queryParameters MatchedRoute:(NSString *)matchedRoute AndRouteParameters:(NSDictionary *)routeParameters {
    // Create a new request
    TPRouteRequest *request = [[TPRouteRequest alloc] init];
    // Set instance variables
    request->_urlSchema = urlSchema;
    request->_queryParameters = queryParameters;
    request->_matchedRoute = matchedRoute;
    request->_routeParameters = routeParameters;
    // Return request
    return request;
}

@synthesize urlSchema=_urlSchema;
@synthesize queryParameters=_queryParameters;
@synthesize matchedRoute=_matchedRoute;
@synthesize routeParameters=_routeParameters;


@end
