//
//  TPFilterChain.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPFilterChain.h"

@interface TPFilterChain()

@property (strong, nonatomic) NSArray *filters;
@property (strong, nonatomic) TPRouteDestination callback;

@end

@implementation TPFilterChain

+ (void) dofilterChainWithFilters:(NSArray *)filters Request:(TPRouteRequest *)request AndCallback:(TPRouteDestination)callback {
    TPFilterChain *filterChain = [[TPFilterChain alloc] init];
    filterChain.filters = filters;
    filterChain.callback = callback;
    [filterChain doFilterWithRequest:request];
}

@synthesize filters=_filters;
@synthesize callback=_callback;

- (void) doFilterWithRequest:(TPRouteRequest *)request {
    // If there are more filters to run, lets run the next one
    if ([self.filters count] != 0) {
        // Grab the next filter
        id<TPFilterProtocol> nextFilter = [self.filters objectAtIndex:0];
        
        // Trim the first elemnt off of our filters list
        NSRange subarrayRange;
        subarrayRange.location = 1;
        subarrayRange.length = [self.filters count]-1;
        self.filters = [self.filters subarrayWithRange:subarrayRange];
        
        // Perform the next filter
        [nextFilter doFilterWithRequest:request AndFilterChain:self];
        
    }
    // No more filters, lets execute our route's callback
    else {
        // Execute the route callback
        self.callback(request);
    }
}

@end
