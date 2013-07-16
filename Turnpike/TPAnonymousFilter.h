//
//  TPAnonymousFilter.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TPFilterProtocol.h"

@class TPFilterChain;

/**
 An alias for `(^TPFilterBlock)(TPRouteRequest *request, TPFilterChain *filterChain)`. Used as the block for processing filters.
 */
typedef void (^TPFilterBlock)(TPRouteRequest *request, TPFilterChain *filterChain);

@interface TPAnonymousFilter : NSObject <TPFilterProtocol>

///-------------------------------
/// @name Factory Method
///-------------------------------
/**
 Create a `TPAnonymousFilter` with a `TPFilterBlock`.
 @param filterBlock The `TPFilterBlock` to perform as the filter logic.
 @return The new `TPAnonymousFilter` object as an `id<TPFilterProtocol>`.
 */
+ (id<TPFilterProtocol>) filterWithBlock:(TPFilterBlock)filterBlock;

@end