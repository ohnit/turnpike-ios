//
//  URIHelperTests.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/17/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "URIHelperTests.h"
#import "TPURIHelper.h"

@implementation URIHelperTests

-(void) assertString:(NSString *)inputString EqualsScheme:(NSString *)scheme Path:(NSString *)path AndQuery:(NSString *)query {
    NSURL *outputURL = [TPURIHelper sanitizeString:inputString];
    STAssertEquals(scheme, outputURL.scheme, @"should be equal to the test scheme");
    STAssertEquals(path, outputURL.path, @"should be equal to the test path");
    STAssertEquals(query, outputURL.query, @"should be equal to the test query");
}

-(void) testSanitizeStringWithEmptyRootPath {
    [self assertString:@"" EqualsScheme:nil Path:@"/" AndQuery:nil];
}

@end
