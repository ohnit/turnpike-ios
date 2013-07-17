//
//  TPURIHelper.m
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import "TPURIHelper.h"

#define LEADING_SLASHES [NSRegularExpression regularExpressionWithPattern:@"^//+"]
#define SINGLE_SLASH @"/"
#define COLON @":"

// "every non-slash character up until the first ':' character
#define URI_SCHEME [NSRegularExpression regularExpressionWithPattern:@"^[^/]+:"]
#define EMPTY_STRING @""
#define AMPERSAND @"&"
#define EQUALS @"="

@implementation TPURIHelper

@end
