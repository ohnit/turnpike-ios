//
//  TurnpikeTests.h
//  TurnpikeTests
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>

/**
 Tests for Turnpike
 */

@interface TurnpikeTests : SenTestCase

///-------------------------------
/// @name Route & Invocation Tests
///-------------------------------

/**
 Test a basic route with a variable.
 */
- (void)testBasicRoute;
/**
 Test a route that is not defined.
 */
- (void)testEmptyRoute;
/**
 Test setting a default (fallback) route, and having it execute when invoking an undefined route.
 */
- (void)testDefaultRoute;
/**
 Test two separate routers, just to prove theres no singleton magic behind Turnpike.
 */
- (void)testSeparateRouters;

///-------------------------------
/// @name Filter Tests
///-------------------------------

/**
 Test a basic filter and detecting internal vs external routes with the request's schema property
 */
- (void)testBasicFilter;

/**
 Test creating anonymous filters with blocks
 */
- (void)testAnonymousFilters;

/**
 Test execution of filter chain in the order that filters are added to a router
 */
- (void)testFilterChainOrder;

@end
