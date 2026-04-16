/*
 * Host-side physics/collision tests for @rkanoid modern.
 *
 * Builds with host gcc via `make test` in @rkanoid/current-modern. Validates the fixed-point
 * ball motion, wall/paddle/brick resolution, and sub-step integration against a set of
 * deterministic scenarios. No hardware dependencies.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "game_types.h"
#include "gba_types.h"
#include "game_constants.h"
#include "game_state.h"
#include "fixed_point.h"
#include "ball_motion.h"
#include "levels.h"
#include "collisions.h"
#include "physics.h"

Block gBlocks[162];

static int gTestsRun = 0;
static int gTestsFailed = 0;
static const char *gCurrentTest = "";

#define TEST_CASE(name) \
    static void name(void); \
    static void name##_wrapper(void) { gCurrentTest = #name; name(); } \
    static void name(void)

#define CHECK(cond) \
    do { \
        gTestsRun++; \
        if (!(cond)) { \
            gTestsFailed++; \
            fprintf(stderr, "  FAIL %s:%d in %s: %s\n", __FILE__, __LINE__, gCurrentTest, #cond); \
        } \
    } while (0)

#define CHECK_EQ(actual, expected) \
    do { \
        long _a = (long)(actual); \
        long _e = (long)(expected); \
        gTestsRun++; \
        if (_a != _e) { \
            gTestsFailed++; \
            fprintf(stderr, "  FAIL %s:%d in %s: %s == %s (got %ld, expected %ld)\n", __FILE__, __LINE__, \
                gCurrentTest, #actual, #expected, _a, _e); \
        } \
    } while (0)

#define CHECK_NEAR(actual, expected, tolerance) \
    do { \
        long _a = (long)(actual); \
        long _e = (long)(expected); \
        long _t = (long)(tolerance); \
        long _d = _a - _e; \
        if (_d < 0) \
            _d = -_d; \
        gTestsRun++; \
        if (_d > _t) { \
            gTestsFailed++; \
            fprintf(stderr, "  FAIL %s:%d in %s: |%s - %s| <= %ld (got diff %ld)\n", __FILE__, __LINE__, \
                gCurrentTest, #actual, #expected, _t, _d); \
        } \
    } while (0)

static void ResetRuntime(RuntimeState *runtime)
{
    runtime->ballWidth = 6;
    runtime->ballHeight = 6;
    runtime->paddleWidth = 32;
    runtime->paddleHeight = 8;
    runtime->maxBallY = 160;
    runtime->ballTileBase = 0;
    runtime->longPaddle = FALSE;
    runtime->trailEnabled = FALSE;
    runtime->shieldActive = FALSE;
}

static void ResetFrame(FrameState *frame)
{
    memset(frame, 0, sizeof(*frame));
    frame->paddleX = 80;
    frame->paddleY = 144;
}

static void ResetSession(GameSession *session)
{
    session->playerCount = 1;
    session->activePlayer = PLAYER_ONE;
    session->lives = 3;
    session->level = 1;
    session->score = 0;
    session->levelTileMap = NULL;
}

static void ResetAllBlocks(void)
{
    u16 i;
    for (i = 0; i < 162; i++) {
        gBlocks[i].x = 0;
        gBlocks[i].y = 0;
        gBlocks[i].right = 0;
        gBlocks[i].bottom = 0;
        gBlocks[i].tileIndex = 0;
        gBlocks[i].tileIndex2 = 0;
        gBlocks[i].destructible = FALSE;
        gBlocks[i].hitsRemaining = 0;
        gBlocks[i].bonusType = NO_BONUS;
        gBlocks[i].bonusActive = FALSE;
        gBlocks[i].bonusX = 0;
        gBlocks[i].bonusY = 0;
        gBlocks[i].bonusSlot = 0;
    }
}

static void SetBlock(u16 index, s32 x, s32 y)
{
    gBlocks[index].x = x;
    gBlocks[index].y = y;
    gBlocks[index].right = x + 16;
    gBlocks[index].bottom = y + 8;
    gBlocks[index].destructible = TRUE;
    gBlocks[index].hitsRemaining = 1;
    gBlocks[index].bonusType = NO_BONUS;
}

static s32 VelMag(s32 vx, s32 vy)
{
    double m = sqrt((double)vx * vx + (double)vy * vy);
    return (s32)m;
}

/* ----------------------------------------------------------------------- */

TEST_CASE(normalize_preserves_speed_mag)
{
    s32 vx = FIX_ONE;
    s32 vy = -FIX_ONE;
    BallNormalizeVelocity(&vx, &vy, BALL_SPEED_MAG);
    CHECK_NEAR(VelMag(vx, vy), BALL_SPEED_MAG, 2);
}

TEST_CASE(normalize_tiny_velocity_falls_back_to_diagonal)
{
    s32 vx = 0;
    s32 vy = 0;
    BallNormalizeVelocity(&vx, &vy, BALL_SPEED_MAG);
    CHECK(vx != 0);
    CHECK(vy < 0);
    CHECK_NEAR(VelMag(vx, vy), BALL_SPEED_MAG, 2);
}

TEST_CASE(right_wall_bounces_and_snaps_inside)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    frame.ballPosX = FIX_FROM_INT(MAX_X - runtime.ballWidth);
    frame.ballPosY = FIX_FROM_INT(60);
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = -FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ApplyWorldBounds(&frame, &runtime, &session, &lostLife);
    CHECK_EQ(lostLife, 0);
    CHECK(frame.ballVelX < 0);
    CHECK(frame.ballX < (MAX_X - runtime.ballWidth));
}

TEST_CASE(left_wall_bounces_and_snaps_inside)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    frame.ballPosX = FIX_FROM_INT(MIN_X);
    frame.ballPosY = FIX_FROM_INT(60);
    frame.ballVelX = -FIX_ONE;
    frame.ballVelY = -FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ApplyWorldBounds(&frame, &runtime, &session, &lostLife);
    CHECK_EQ(lostLife, 0);
    CHECK(frame.ballVelX > 0);
    CHECK(frame.ballX > MIN_X);
}

TEST_CASE(ceiling_bounces_and_snaps_inside)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    frame.ballPosX = FIX_FROM_INT(80);
    frame.ballPosY = FIX_FROM_INT(MIN_Y);
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = -FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ApplyWorldBounds(&frame, &runtime, &session, &lostLife);
    CHECK_EQ(lostLife, 0);
    CHECK(frame.ballVelY > 0);
    CHECK(frame.ballY > MIN_Y);
}

TEST_CASE(death_floor_loses_life_without_shield)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    frame.ballPosX = FIX_FROM_INT(80);
    frame.ballPosY = FIX_FROM_INT(runtime.maxBallY - runtime.ballHeight);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ApplyWorldBounds(&frame, &runtime, &session, &lostLife);
    CHECK_EQ(lostLife, 1);
    CHECK_EQ(session.lives, 2);
}

TEST_CASE(shield_bounces_and_deactivates)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    runtime.shieldActive = TRUE;
    runtime.maxBallY = 148;
    frame.ballPosX = FIX_FROM_INT(80);
    frame.ballPosY = FIX_FROM_INT(runtime.maxBallY - runtime.ballHeight);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ApplyWorldBounds(&frame, &runtime, &session, &lostLife);
    CHECK_EQ(lostLife, 0);
    CHECK_EQ(runtime.shieldActive, FALSE);
    CHECK_EQ(runtime.maxBallY, 160);
    CHECK(frame.ballVelY < 0);
}

TEST_CASE(paddle_center_hit_is_vertical)
{
    FrameState frame;
    RuntimeState runtime;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    /* Ball centered above paddle, one pixel into the paddle top row. */
    frame.paddleX = 80;
    frame.ballPosX = FIX_FROM_INT(frame.paddleX + (s32)runtime.paddleWidth / 2 - (s32)runtime.ballWidth / 2);
    frame.ballPosY = FIX_FROM_INT(frame.paddleY - (s32)runtime.ballHeight + 1);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ApplyPaddleCollision(&frame, &runtime);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelY < 0);
    /* Vertical launch: lateral component should be tiny or zero. */
    CHECK_NEAR(frame.ballVelX, 0, 16);
    CHECK_NEAR(VelMag(frame.ballVelX, frame.ballVelY), BALL_SPEED_MAG, 3);
    /* Ball sits on top of the paddle, not overlapping. */
    CHECK_EQ(frame.ballY, frame.paddleY - runtime.ballHeight);
}

TEST_CASE(paddle_right_edge_hits_launches_up_right)
{
    FrameState frame;
    RuntimeState runtime;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    frame.paddleX = 80;
    frame.ballPosX = FIX_FROM_INT(frame.paddleX + (s32)runtime.paddleWidth - 2);
    frame.ballPosY = FIX_FROM_INT(frame.paddleY - (s32)runtime.ballHeight + 1);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ApplyPaddleCollision(&frame, &runtime);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelX > 0);
    CHECK(frame.ballVelY < 0);
}

TEST_CASE(paddle_left_edge_hits_launches_up_left)
{
    FrameState frame;
    RuntimeState runtime;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    frame.paddleX = 80;
    frame.ballPosX = FIX_FROM_INT(frame.paddleX - 2);
    frame.ballPosY = FIX_FROM_INT(frame.paddleY - (s32)runtime.ballHeight + 1);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ApplyPaddleCollision(&frame, &runtime);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelX < 0);
    CHECK(frame.ballVelY < 0);
}

TEST_CASE(brick_top_face_flips_vy_and_snaps_above)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 20);
    frame.ballPosX = FIX_FROM_INT(48);
    frame.ballPosY = FIX_FROM_INT(15); /* ballB = 20 = block->y, overlap by 1 */
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    /* Make brick multi-hit so it survives this test and we can check snap geometry. */
    gBlocks[0].hitsRemaining = 3;

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelY < 0);
    /* Ball snapped out just above brick */
    CHECK(frame.ballY + runtime.ballHeight - 1 < (u16)gBlocks[0].y);
    CHECK_NEAR(VelMag(frame.ballVelX, frame.ballVelY), BALL_SPEED_MAG, 3);
}

TEST_CASE(brick_bottom_face_flips_vy_and_snaps_below)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 20);
    gBlocks[0].hitsRemaining = 3;
    frame.ballPosX = FIX_FROM_INT(48);
    frame.ballPosY = FIX_FROM_INT(27); /* ballT = 27, block->bottom = 28, overlap by 1 */
    frame.ballVelX = 0;
    frame.ballVelY = -FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelY > 0);
    /* Ball snapped out below brick */
    CHECK(frame.ballY >= (u16)gBlocks[0].bottom);
    CHECK_NEAR(VelMag(frame.ballVelX, frame.ballVelY), BALL_SPEED_MAG, 3);
}

TEST_CASE(brick_left_face_flips_vx_and_snaps_left)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 20);
    gBlocks[0].hitsRemaining = 3;
    frame.ballPosX = FIX_FROM_INT(35); /* ballR = 40 = block->x, overlap by 1 */
    frame.ballPosY = FIX_FROM_INT(24);
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = 0;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelX < 0);
    CHECK(frame.ballX + runtime.ballWidth - 1 < (u16)gBlocks[0].x);
}

TEST_CASE(brick_right_face_flips_vx_and_snaps_right)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 20);
    gBlocks[0].hitsRemaining = 3;
    frame.ballPosX = FIX_FROM_INT(55); /* ballL = 55, block->right = 56, overlap by 1 */
    frame.ballPosY = FIX_FROM_INT(24);
    frame.ballVelX = -FIX_ONE;
    frame.ballVelY = 0;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    CHECK(frame.ballVelX > 0);
    CHECK(frame.ballX >= (u16)gBlocks[0].right);
}

TEST_CASE(brick_top_row_snap_does_not_warp_to_ceiling_far)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 16); /* top-row brick */
    gBlocks[0].hitsRemaining = 3;
    frame.ballPosX = FIX_FROM_INT(48);
    frame.ballPosY = FIX_FROM_INT(11); /* ballB = 16 */
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    /* Ball must remain above the brick and in-field (not at MIN_Y+1 = 9 = ceiling clamp). */
    CHECK(frame.ballY + runtime.ballHeight - 1 < (u16)gBlocks[0].y);
    CHECK(frame.ballY >= MIN_Y);
    CHECK(frame.ballY < 12); /* within a pixel of original y=11 */
}

TEST_CASE(brick_corner_deep_penetration_resolves_vertically)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    SetBlock(0, 40, 20);
    /* Ball slightly inside top-left corner but penetrated more vertically than horizontally. */
    frame.ballPosX = FIX_FROM_INT(36); /* ballR = 41: pen-left = 2 */
    frame.ballPosY = FIX_FROM_INT(17); /* ballB = 22: pen-top = 3 */
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    /* penLeft (2) < penTop (3): MTV picks horizontal, vx should flip. */
    CHECK(frame.ballVelX < 0);
}

TEST_CASE(brick_pierce_with_trail_destroys_and_keeps_velocity)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    runtime.trailEnabled = TRUE;
    SetBlock(0, 40, 20);
    frame.ballPosX = FIX_FROM_INT(48);
    frame.ballPosY = FIX_FROM_INT(15);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    s32 vxBefore = frame.ballVelX;
    s32 vyBefore = frame.ballVelY;
    Ball_SyncPixelsFromFixedPos(&frame);

    boolean hit = ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK_EQ(hit, TRUE);
    CHECK_EQ(frame.ballVelX, vxBefore);
    CHECK_EQ(frame.ballVelY, vyBefore);
    CHECK_EQ(gBlocks[0].x, 0); /* destroyed */
}

TEST_CASE(integrate_keeps_constant_speed_across_frames)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    u8 tileMap[1024] = {0};
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    ResetAllBlocks();
    frame.ballPosX = FIX_FROM_INT(80);
    frame.ballPosY = FIX_FROM_INT(60);
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = FIX_ONE;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    int i;
    for (i = 0; i < 20; i++) {
        IntegrateBallMotionThisFrame(&frame, &runtime, &session, &lostLife, tileMap, &session.score);
        CHECK_EQ(lostLife, 0);
        CHECK_NEAR(VelMag(frame.ballVelX, frame.ballVelY), BALL_SPEED_MAG, 4);
    }
}

TEST_CASE(integrate_steep_velocity_catches_brick)
{
    FrameState frame;
    RuntimeState runtime;
    GameSession session;
    int lostLife = 0;
    u8 tileMap[1024] = {0};
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetSession(&session);
    ResetAllBlocks();
    SetBlock(0, 40, 40);
    /* Aim from y=30 straight down into brick top (y=40). */
    frame.ballPosX = FIX_FROM_INT(48);
    frame.ballPosY = FIX_FROM_INT(30);
    frame.ballVelX = 0;
    frame.ballVelY = FIX_FROM_INT(1); /* 1 px/frame, safe */
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    int i;
    int hitBrick = 0;
    for (i = 0; i < 15; i++) {
        IntegrateBallMotionThisFrame(&frame, &runtime, &session, &lostLife, tileMap, &session.score);
        if (gBlocks[0].x == 0) {
            hitBrick = 1;
            break;
        }
    }
    CHECK_EQ(hitBrick, 1);
}

TEST_CASE(ball_never_lands_outside_playfield_after_snap)
{
    FrameState frame;
    RuntimeState runtime;
    u8 tileMap[1024] = {0};
    u32 score = 0;
    ResetFrame(&frame);
    ResetRuntime(&runtime);
    ResetAllBlocks();
    /* Brick at bottom-left corner of the field (edge case). */
    SetBlock(0, MIN_X, 16);
    frame.ballPosX = FIX_FROM_INT(MIN_X); /* overlap brick's left side */
    frame.ballPosY = FIX_FROM_INT(20);
    frame.ballVelX = FIX_ONE;
    frame.ballVelY = 0;
    BallNormalizeVelocity(&frame.ballVelX, &frame.ballVelY, BALL_SPEED_MAG);
    Ball_SyncPixelsFromFixedPos(&frame);

    ProcessBlockCollisions(&frame, &runtime, tileMap, &score);
    CHECK(frame.ballX >= MIN_X);
    CHECK(frame.ballX <= MAX_X - runtime.ballWidth);
    CHECK(frame.ballY >= MIN_Y);
    CHECK(frame.ballY <= runtime.maxBallY - runtime.ballHeight);
}

/* ----------------------------------------------------------------------- */

typedef void (*TestFn)(void);

int main(void)
{
    TestFn tests[] = {
        normalize_preserves_speed_mag_wrapper,
        normalize_tiny_velocity_falls_back_to_diagonal_wrapper,
        right_wall_bounces_and_snaps_inside_wrapper,
        left_wall_bounces_and_snaps_inside_wrapper,
        ceiling_bounces_and_snaps_inside_wrapper,
        death_floor_loses_life_without_shield_wrapper,
        shield_bounces_and_deactivates_wrapper,
        paddle_center_hit_is_vertical_wrapper,
        paddle_right_edge_hits_launches_up_right_wrapper,
        paddle_left_edge_hits_launches_up_left_wrapper,
        brick_top_face_flips_vy_and_snaps_above_wrapper,
        brick_bottom_face_flips_vy_and_snaps_below_wrapper,
        brick_left_face_flips_vx_and_snaps_left_wrapper,
        brick_right_face_flips_vx_and_snaps_right_wrapper,
        brick_top_row_snap_does_not_warp_to_ceiling_far_wrapper,
        brick_corner_deep_penetration_resolves_vertically_wrapper,
        brick_pierce_with_trail_destroys_and_keeps_velocity_wrapper,
        integrate_keeps_constant_speed_across_frames_wrapper,
        integrate_steep_velocity_catches_brick_wrapper,
        ball_never_lands_outside_playfield_after_snap_wrapper,
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    size_t i;
    for (i = 0; i < n; i++)
        tests[i]();

    printf("Tests: %d checks, %d failures\n", gTestsRun, gTestsFailed);
    return gTestsFailed == 0 ? 0 : 1;
}
