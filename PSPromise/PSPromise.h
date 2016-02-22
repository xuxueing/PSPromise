//
//  PSPromise.h
//  PSPromise
//
//  Created by PoiSon on 16/2/15.
//  Copyright © 2016年 PoiSon. All rights reserved.
//

#import <Foundation/Foundation.h>

#define PSPROMISE_API_UNAVAILABLE(INFO) __attribute__((unavailable(INFO)))

NS_ASSUME_NONNULL_BEGIN
typedef void (^PSResolve)(id __nullable result);

typedef NS_ENUM(NSUInteger, PSPromiseState) {
    PSPromiseStatePending,
    PSPromiseStateFulfilled,
    PSPromiseStateRejected
};

@interface PSPromise : NSObject
- (instancetype)init PSPROMISE_API_UNAVAILABLE("不允许直接实例化");
+ (instancetype)new PSPROMISE_API_UNAVAILABLE("不允许直接实例化");

@property (nonatomic, readonly, assign) PSPromiseState state;
@property (nonatomic, readonly) id value;

- (PSPromise *(^)(id block))finally;
- (PSPromise *(^)(void (^resolver)(id result, PSResolve resolve)))thenPromise;
@end

@interface PSPromise (CommonJS)
/**
 *  接受的参数如果是promise对象就直接返回
 *  如果接受的参数是NSError对象，就会生成一个失败态(rejected)的promise，并传递给之后的catch
 *  其它的会生成一个成功态(fulfilled)的promise，并传递给之后的then
 */
+ (PSPromise *(^)(id value))resolve;

/**
 *  PSPromise.all用来包装一系列的promise对象，返回一个包装后的promise对象，我们称之为A
 *  1. 当所有的promise对象都变成成功态(fulfilled)后，这个包装后的A才会把自己变成成功状态。
 *    A会等最慢的那个promise对象变成成功态(fulfilled)后才把自己变成成功态。
 *  2. 只要其中一个promise对象变成失败态(rejected)，包装后的A就变成rejected，
 *    并且第一个rejected传递的值，会传递给A后面的catch。
 */
+ (PSPromise *(^)(NSArray<PSPromise *> *promises))all;
/**
 *  PSPromise.race用来包装一系列的promise对象，返回一个包装后的promise对象，我们称之为R
 *  1. 只要其中的一个promise对象变成成功态(fulfilled)后，这个包装后的R就会变成成功态(fulfilled)，
 *    并且其它的promise不再执行。
 *  2. 当所有的promise对象都变成失败态(rejected)后，这个包装后的R才会把自己变成失败状态。
 */
+ (PSPromise *(^)(NSArray<PSPromise *> *promises))race;

/**
 *  then接受成功回调
 *  如果promise对象处于预备状态就等待，直到状态改变才开始执行
 *  如果promise对象处于成功态，再用then添加回调就直接调用对应的回调
 *  如果then的返回值不是promise，会作为下一个then的参数
 *  如果then的返回值是一个新的promise对象，那么之后的then添加的操作函数会被托管给新的promise对象
 */
- (PSPromise *(^)(id block))then;

/**
 *  catch接受失败回调
 *  如果promise对象处于预备状态就等待，直到状态改变才开始执行
 *  如果promise对象处于失败态，再用catch添加回调就直接调用对应的回调
 *  如果catch的返回值不是promise，会作为下一个then的参数
 *  如果catch的返回值是一个新的promise对象，那么之后的then添加的操作函数会被托管给新的promise对象
 */
- (PSPromise *(^)(id block))catch;
@end

FOUNDATION_EXPORT PSPromise *PSPROMISE(void (^)(PSResolve resolve));

NS_ASSUME_NONNULL_END


