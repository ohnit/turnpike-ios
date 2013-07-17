//
//  TPParsingHelper.h
//  Turnpike
//
//  Created by James Lawrence Turner on 7/16/13.
//  Copyright (c) 2013 URX. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TPParsingHelper : NSObject

+ (NSString *)sanitizeMappedPath:(NSString *)path;
+ (void)validateDispatchedPath:(NSString *)dispatchedPath error:(NSError **)error;

@end
