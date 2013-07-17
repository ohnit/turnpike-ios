//
//  ParsingHelperTests.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "ParsingHelperTests.h"
#import "TPParsingHelper.h"

@implementation ParsingHelperTests

-(void)testValidateLeadingTrailingSlashes {
    ^{
        NSError *error;
        [TPParsingHelper validateDispatchedPath:@"about/team" error:&error];
        STAssertNil(error, @"should be a valid path");
    }();
    
    ^{
        NSError *error;
        [TPParsingHelper validateDispatchedPath:@"/about/team" error:&error];
        STAssertNotNil(error, @"should not be valid with a leading slash");
    }();
    
    ^{
        NSError *error;
        [TPParsingHelper validateDispatchedPath:@"about/team/" error:&error];
        STAssertNotNil(error, @"should not be valid with a trailing slash");
    }();
}



@end
