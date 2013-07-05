//
//  TPAnonymousFilter.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPAnonymousFilter.h"
#import "TPFilterChain.h"

@interface TPAnonymousFilter()

@property (strong, nonatomic) TPFilterBlock filterBlock;

@end

@implementation TPAnonymousFilter

+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock {
    TPAnonymousFilter *filter = [[TPAnonymousFilter alloc]init];
    filter.filterBlock = filterBlock;
    return filter;
}

@synthesize filterBlock=_filterBlock;

- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
    if (self.filterBlock) {
        self.filterBlock(request, filterChain);
    }
    else {
        [filterChain doFilterWithRequest:request];
    }
}

@end