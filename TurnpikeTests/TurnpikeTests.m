//
//  TurnpikeTests.m
//  TurnpikeTests
//
//  Created by James Lawrence Turner on 7/5/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TurnpikeTests.h"
#import "Turnpike.h"

/**
 A Test Filter, implementing TPFilterProtocol, used in testBasicFilter
 */
@interface TestFilter : NSObject <TPFilterProtocol>
@property (strong,nonatomic) NSString *schema;
@end

@implementation TestFilter
@synthesize schema=_schema;
- (void) doFilterWithRequest:(TPRouteRequest *)request AndFilterChain:(TPFilterChain *)filterChain {
    // Set schema to the external url schema of the route request
    self.schema = request.urlSchema;
    
    // Continue the filter chain
    [filterChain doFilterWithRequest:request];
}
@end

@implementation TurnpikeTests

- (void)testBasicRoute {
    // The router we'll test on
    TPRouter *router = [TPRouter router];
    
    // This is what we should get back
    NSString *desiredUserID = @"4";
    // This will be what we do get back
    __block NSString *receivedUserID = nil;
    // Create a users mapping
    [router mapRoute:@"users/:user_id" ToDestination:^(TPRouteRequest *request) {
        // Set the user id we get from our route
        receivedUserID = [request.routeParameters valueForKey:@"user_id"];
    }];
    
    // Invoke our route with our desired user id
    [router resolveURL:[NSURL URLWithString:[NSString stringWithFormat:@"users/%@",desiredUserID]]];
    
    // Assert that the user id we got in our route is the same as our desired user id
    STAssertTrue([desiredUserID isEqualToString:receivedUserID], [NSString stringWithFormat:@"Should have received a user id of %@, instead received %@", desiredUserID, receivedUserID]);
}

- (void)testABasicStrangeBrokenRoute {
    // The router we'll test on
    TPRouter *router = [TPRouter router];
    
    // This is what we should get back
    NSString *desiredUserID = @"4";
    // This will be what we do get back
    __block NSString *receivedUserID = nil;
    // Create a users mapping
    [router mapRoute:@"users/:user_id" ToDestination:^(TPRouteRequest *request) {
        // Set the user id we get from our route
        receivedUserID = [request.routeParameters valueForKey:@"user_id"];
    }];
    
    // Invoke our route with our desired user id
    [router resolveURL:[NSURL URLWithString:[NSString stringWithFormat:@"/////////////////////users////////////////////////%@///////////////",desiredUserID]]];
    
    // Assert that the user id we got in our route is the same as our desired user id
    STAssertTrue([desiredUserID isEqualToString:receivedUserID], [NSString stringWithFormat:@"Should have received a user id of %@, instead received %@", desiredUserID, receivedUserID]);
}

- (void)testEmptyRoute {
    // The router we'll test on
    TPRouter *router = [TPRouter router];
    
    // This value should remain nil
    __block NSString *shouldBeNil = nil;
    
    // This is a route which should never be called
    [router mapRoute:@"should/never/be/called" ToDestination:^(TPRouteRequest *request) {
        // This will change our variable which should not change, because this should never get run
        shouldBeNil = @"clearly not nil";
    }];
    
    // Invoke a route that does not exist
    [router resolveURL:[NSURL URLWithString:@"a/route/that/does/not/exist"]];
    
    // Assert that our string is still nil
    STAssertNil(shouldBeNil, @"Test string should be nil");
}

- (void)testAStrangeBrokenRoute {
    // The router we'll test on
    TPRouter *router = [TPRouter router];
    
    // This value should remain nil
    __block NSString *shouldBeNil = nil;
    
    // This is a route which should never be called
    [router mapRoute:@"should/never/be/called" ToDestination:^(TPRouteRequest *request) {
        // This will change our variable which should not change, because this should never get run
        shouldBeNil = @"clearly not nil";
    }];
    
    // Invoke a route that does not exist
    [router resolveURL:[NSURL URLWithString:@"broken:::///////////////nooooo/:::::::::::::::::::::::yes///////////"]];
    
    // Assert that our string is still nil
    STAssertNil(shouldBeNil, @"Test string should be nil");
}

- (void)testDefaultRoute {
    // The router we'll test on
    TPRouter *router = [TPRouter router];
    
    __block NSString *matchedRoute = @"this should be nil because our route will not exist";
    
    // Set the callback for the default route to set matched route to the route it matches (which should be nil)
    [router mapDefaultToDestination:^(TPRouteRequest *request) {
        // This should set matchedRoute to nil
        matchedRoute = request.matchedRoute;
    }];
    
    // Invoke a route that does not exist
    [router resolveURL:[NSURL URLWithString:@"a/route/that/does/not/exist"]];
    
    // Assert that matchedRoute is nil
    STAssertNil(matchedRoute, [NSString stringWithFormat:@"Matched Route should be nil, instead it's \"%@\"", matchedRoute]);
}

- (void)testSeparateRouters {
    // This is the value that our routers will set and we will test
    __block NSString *returnedValue = nil;
    
    // What the first router should return
    NSString *mainRouterValue = @"apples";
    // What the second router should return
    NSString *otherRouterValue = @"oranges";
    
    // The first router
    TPRouter *mainRouter = [TPRouter router];
    // The second router
    TPRouter *otherRouter = [TPRouter router];
    
    // Setup our route in the first router
    [mainRouter mapRoute:@"test/route" ToDestination:^(TPRouteRequest *request) {
        returnedValue = mainRouterValue;
    }];
    
    // Setup our router in the second router
    [otherRouter mapRoute:@"test/route" ToDestination:^(TPRouteRequest *request) {
        returnedValue = otherRouterValue;
    }];
    
    // Invoke our route on the first router
    [mainRouter resolveURL:[NSURL URLWithString:@"test/route"]];
    // Assert that the first router's value was set
    STAssertEquals(returnedValue, mainRouterValue, @"Returned value should be equal to the main router value after routing");
    
    // Invoke our route on the second router
    [otherRouter resolveURL:[NSURL URLWithString:@"test/route"]];
    // Assert that the second router's value was set
    STAssertEquals(returnedValue, otherRouterValue, @"Returned value should be equal to the other router value after routing");
}

- (void) testBasicFilter {
    // This is the external url schema to test
    NSString *urlSchema = @"myURLSchema";
    // This will be the nonexistant route to invoke
    NSString *route = @"an/awesome/route";
    
    // We'll create a new router to add filters to
    TPRouter *router = [TPRouter router];
    
    // This is our test filter, as defined at the top of TurnpikeTests.m
    TestFilter *testFilter = [[TestFilter alloc] init];
    // Add the filter to our router
    [router appendFilter:testFilter];
    // Make sure our filter's schema variable is nil, as it should not be initialized
    STAssertNil(testFilter.schema, @"Test Filter schema should be nil because we haven't run an external route yet.");
    
    // Invoke our external route, and our TestFilter should catch the external url schema
    [router resolveURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@:%@", urlSchema, route]]];
    // Make sure we got the schema
    STAssertTrue([testFilter.schema isEqualToString:urlSchema], @"Test Filter scheme should be equal to the external URL schema used to invoke the route");
    
    // Invoke an internal route, which does not have a schema
    [router resolveURL:[NSURL URLWithString:route]];
    // Schema should be nil because our filter just processed an internal invocation request
    STAssertNil(testFilter.schema, @"Internally invoked routes should not have a schema, it should be nil");
}

- (void)testAnonymousFilters {
    // Our anonymous filter will set this value
    __block NSString *testValue = nil;
    // This is the value our anonymous filter should be set too
    NSString *desiredValue = @"1234";
    
    // Create a new router to add filters to
    TPRouter *router = [TPRouter router];
    
    // Create and add our anonymous filter to test
    [router appendAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        testValue = [request.routeParameters valueForKey:@"value"];
        
        [filterChain doFilterWithRequest:request];
    }];
    
    // Add our test route with our value
    [router mapRoute:@"test/route/with/:value" ToDestination:^(TPRouteRequest *request) {}];
    
    // Invoke our route with our desired value
    [router resolveURL:[NSURL URLWithString:[NSString stringWithFormat:@"test/route/with/%@", desiredValue]]];
    
    STAssertTrue([testValue isEqualToString: desiredValue], @"Anonymous filter should have set test value to desired value");
}

- (void)testFilterChainOrder {
    __block NSString *testValue = nil;
    NSString *filterValueOne = @"one";
    NSString *filterValueTwo = @"two";
    
    // Create a new router to add filters to
    TPRouter *router = [TPRouter router];
    
    // Create our first filter
    [router appendAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        // Set testValue to the first value
        testValue = filterValueOne;
        // Continue the filter chain
        [filterChain doFilterWithRequest:request];
    }];
    
    // Because the route hasn't been invoked yet, the filter will not have run, so test value should still be nil
    STAssertNil(testValue, @"Before invoking with our filters, the test value should be nil");
    
    // Run a route to run our filter
    [router resolveURL:[NSURL URLWithString:@"some/route"]];
    
    // The test value should be equal to the first filter value, set by our first filter
    STAssertEquals(testValue, filterValueOne, @"With only the first filter, the test value should be equal to filter value one");
    
    // Create our second filter
    [router appendAnonymousFilter:^(TPRouteRequest *request, TPFilterChain *filterChain) {
        // Set testValue to the second value
        testValue = filterValueTwo;
        // Continue the filter chain
        [filterChain doFilterWithRequest:request];
    }];
    
    // Run a route to run our filters
    [router resolveURL:[NSURL URLWithString:@"some/route"]];
    
    // The test value should not be equal to the first filter value because we've added a second filter that should have changed the test value after the first filter
    STAssertFalse([filterValueOne isEqualToString:testValue], @"With our second filter, the test value should be equal to filter value two, not filter value one.");
    // The test value should be equal to the second filter value, set by our second filter
    STAssertEquals(testValue, filterValueTwo, @"With our second filter, the test value should be equal to filter value two.");
}

@end
