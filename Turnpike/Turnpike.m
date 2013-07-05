//
//  Turnpike.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "Turnpike.h"
#import "TPHelper.h"

@interface Turnpike()

+(TPRouter *)sharedRouter;

@end

@implementation Turnpike

+(TPRouter *)sharedRouter
{
    static dispatch_once_t pred = 0;
    __strong static TPRouter *_sharedRouter = nil;
    dispatch_once(&pred, ^{
        _sharedRouter = [TPRouter router];
    });
    return _sharedRouter;
}

+ (void)mapRoute:(NSString *)format ToCallback:(TPRoutingCallback)callback {
    [[self sharedRouter] mapRoute:format ToCallback:callback];
}

+ (void)mapDefaultToCallback:(TPRoutingCallback)callback {
    [[self sharedRouter] mapDefaultToCallback:callback];
}

+ (void)addFilter:(id <TPFilterProtocol>)filter {
    [[self sharedRouter] addFilter:filter];
}

+ (void)addAnonymousFilter:(TPFilterBlock)filterBlock {
    [[self sharedRouter] addAnonymousFilter:filterBlock];
}

+ (void)invokeInternalRoute:(NSString *)route {
    [[self sharedRouter] invokeInternalRoute:route];
}

+ (void)invokeExternalRouteFromURL:(NSString *)url {
    [[self sharedRouter] invokeExternalRouteFromURL:url];
}

+ (void)invokeExternalURL:(NSString *)url {
    [TPHelper invokeExternalURL:url];
}

+ (void)invokeExternalAppWithSchema:(NSString *)schema Route:(NSString *)route AndQueryParameters:(NSDictionary *)queryParameters {
    [TPHelper invokeExternalAppWithSchema:schema Route:route AndQueryParameters:queryParameters];
}

+ (BOOL)canInvokeExternalURL:(NSString *)url {
    return [TPHelper canInvokeExternalURL:url];
}

@end
