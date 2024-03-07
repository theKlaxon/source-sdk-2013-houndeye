// Created by Crowbar 0.74

$ModelName "houndeye.mdl"

$BodyGroup "Studio"
{
	studio "ps2_houndeye_lod1.smd"
}


$SurfaceProp "alienflesh"

$Contents "solid"

$IllumPosition 0.863 -4.116 22.343

$EyePosition 0 0 27

$CDMaterials "models\houndeye\"

$TextureGroup "skinfamilies"
{
	{ "eye"  }
	{ "eye1" }
	{ "eye2" }
}

$CBox 0 0 0 0 0 0

$BBox -24.35 -16.639 -1.702 25.011 16.38 45.692

$HBoxSet "default"
$HBox 0 "Bip01 Pelvis" -0.000005 -2.94 -5.5 4.27 4.012748 4.95
$HBox 0 "Bip01 L Leg" -2.1 -7.3 -0.93 14.27 5.3 8.54
$HBox 0 "Bip01 L Leg1" 0 -3.91 0 10.39 2.75 5.35
$HBox 0 "Bip01 L Foot" -0.81 0 0 2.2 4.81 6.2
$HBox 0 "Bip01 Spine" -2.43 -10.23 -8.16 7.11681 10.75 7.87
$HBox 0 "Bip01 Spine1" -4.06 -12.87 -9.06 7.115733 10.16 8.77
$HBox 0 "Bip01 Spine2" -3.66 -15.01 -10.99 7.115576 12.52 10.71
$HBox 0 "Bip01 Spine3" -1.61 -17.209999 -12.73 8.34 11.98 12.44
$HBox 0 "Bip01 Head" -9.73 -1.49 -13.13 16.74 8.95 12.82
$HBox 0 "Bip01 L Arm1" 0 -4.07 -2.49 19.34 9.53 4.93
$HBox 0 "Bip01 L Arm2" 0 -2.01 -1.73 10.41 4.53 2.84
$HBox 0 "Bip01 R Arm1" 0 -5.66 -5.22 19.33 9.53 3.82
$HBox 0 "Bip01 R Arm2" 0 -2.01 -3.15 10.42 4.52 1.46
$HBox 0 "Bone01" -6.18 -2.84 -6.4 2.7 0.8 6.32

$DefineBone "Bip01" "" 0.161057 10.613981 29.008524 0 -89.999927 0 0 0 0 0 0 0
$DefineBone "Bip01 Pelvis" "Bip01" -6.271406 0.000027 -3.741533 -89.980215 -89.999927 0 0 0 0 0 0 0
$DefineBone "Bip01 L Leg" "Bip01 Pelvis" -0.000005 0.000008 3.533232 0.000091 174.625315 -179.999937 0 0 0 0 0 0
$DefineBone "Bip01 L Leg1" "Bip01 L Leg" 14.191055 0 0 0 -24.735103 0 0 0 0 0 0 0
$DefineBone "Bip01 L Foot" "Bip01 L Leg1" 9.483088 0 0 0 19.360419 0 0 0 0 0 0 0
$DefineBone "Bip01 Spine" "Bip01 Pelvis" 1.268482 4.012748 -0.130056 0.000057 75.507359 -0.000286 0 0 0 0 0 0
$DefineBone "Bip01 Spine1" "Bip01 Spine" 7.116809 -0.005523 0 -0.000057 12.993423 0 0 0 0 0 0 0
$DefineBone "Bip01 Spine2" "Bip01 Spine1" 7.115733 -0.005662 0 0 1.998993 0 0 0 0 0 0 0
$DefineBone "Bip01 Spine3" "Bip01 Spine2" 7.115576 -0.004721 0 0 0.499734 0 0 0 0 0 0 0
$DefineBone "Bip01 Neck" "Bip01 Spine3" 5.929615 -0.001504 0 0 0 0 0 0 0 0 0 0
$DefineBone "Bip01 Head" "Bip01 Neck" 1.887411 0.000001 0 0.000172 -90.999567 -0.000286 0 0 0 0 0 0
$DefineBone "Bip01 L Arm" "Bip01 Neck" 0 0.001491 4.397666 -78.540823 179.955158 -0.000802 0 0 0 0 0 0
$DefineBone "Bip01 L Arm1" "Bip01 L Arm" 6.526665 -0.000002 0 10.463871 -86.184939 -79.225625 0 0 0 0 0 0
$DefineBone "Bip01 L Arm2" "Bip01 L Arm1" 19.256117 0 0 0 -25.95178 -0.000001 0 0 0 0 0 0
$DefineBone "Bip01 R Arm" "Bip01 Neck" 0 0.001517 -4.397666 78.540823 179.953614 -0.000802 0 0 0 0 0 0
$DefineBone "Bip01 R Arm1" "Bip01 R Arm" 6.526664 -0.000001 0 -10.46387 -86.184946 79.225625 0 0 0 0 0 0
$DefineBone "Bip01 R Arm2" "Bip01 R Arm1" 19.256119 -0.000001 -0.000001 0 -25.95178 0.000001 0 0 0 0 0 0
$DefineBone "Bone01" "Bip01 Spine3" 4.703579 10.433306 -0.031011 -0.000057 179.000489 -0.000172 0 0 0 0 0 0


$Sequence "ragdoll" {
	"houndeye_anims\ragdoll.smd"
	activity "ACT_DIERAGDOLL" 1
	fadein 0.2
	fadeout 0.2
	fps 2
}

$Sequence "idle1" {
	"houndeye_anims\idle1.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "idle2" {
	"houndeye_anims\idle2.smd"
	activity "ACT_IDLE" 20
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "idle3" {
	"houndeye_anims\idle3.smd"
	activity "ACT_IDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "run" {
	"houndeye_anims\run.smd"
	activity "ACT_RUN" 4
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	walkframe 18 LX
}

$Sequence "run_2" {
	"houndeye_anims\run_2.smd"
	activity "ACT_RUN" 2
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	walkframe 18 LX
}

$Sequence "run_3" {
	"houndeye_anims\run_3.smd"
	activity "ACT_RUN" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	walkframe 18 LX
}

$Sequence "die" {
	"houndeye_anims\die.smd"
	activity "ACT_DIESIMPLE" 1
	{ event 2001 5 "" }
	fadein 0.2
	fadeout 0.2
	fps 20
}

$Sequence "die1" {
	"houndeye_anims\die1.smd"
	activity "ACT_DIEFORWARD" 1
	{ event 2001 10 "" }
	fadein 0.2
	fadeout 0.2
	fps 20
}

$Sequence "die2" {
	"houndeye_anims\die2.smd"
	activity "ACT_DIEBACKWARD" 2
	{ event 2001 8 "" }
	fadein 0.2
	fadeout 0.2
	fps 20
}

$Sequence "die3" {
	"houndeye_anims\die3.smd"
	activity "ACT_DIEBACKWARD" 1
	{ event 2001 7 "" }
	fadein 0.2
	fadeout 0.2
	fps 20
}

$Sequence "attack" {
	"houndeye_anims\attack.smd"
	activity "ACT_RANGE_ATTACK1" 1
	{ event 2 5 "" }
	{ event 3 59 "" }
	fadein 0.2
	fadeout 0.2
	fps 25
}

$Sequence "flinch_small" {
	"houndeye_anims\flinch_small.smd"
	activity "ACT_SMALL_FLINCH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "flinch_small2" {
	"houndeye_anims\flinch_small2.smd"
	activity "ACT_SMALL_FLINCH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "die_crumple" {
	"houndeye_anims\die_crumple.smd"
	activity "ACT_DIESIMPLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "walk_limp" {
	"houndeye_anims\walk_limp.smd"
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	walkframe 22 LX
}

$Sequence "walk2" {
	"houndeye_anims\walk2.smd"
	activity "ACT_WALK" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
	walkframe 35 LX
}

$Sequence "leaderlook" {
	"houndeye_anims\leaderlook.smd"
	activity "ACT_GUARD" 1
	{ event 1 10 "" }
	{ event 1 20 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "sleep" {
	"houndeye_anims\sleep.smd"
	activity "ACT_CROUCHIDLE" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "gotosleep" {
	"houndeye_anims\gotosleep.smd"
	activity "ACT_CROUCH" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "sleeptostand" {
	"houndeye_anims\sleeptostand.smd"
	activity "ACT_STAND" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "madidle" {
	"houndeye_anims\madidle.smd"
	activity "ACT_IDLE_ANGRY" 2
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "madidle2" {
	"houndeye_anims\madidle2.smd"
	activity "ACT_IDLE_ANGRY" 3
	{ event 4 7 "" }
	{ event 7 7 "" }
	{ event 4 17 "" }
	{ event 7 17 "" }
	{ event 5 34 "" }
	{ event 7 34 "" }
	fadein 0.2
	fadeout 0.2
	fps 37
	loop
}

$Sequence "madidle3" {
	"houndeye_anims\madidle3.smd"
	activity "ACT_IDLE_ANGRY" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "inspect" {
	"houndeye_anims\inspect.smd"
	activity "ACT_INSPECT_WALL" 1
	fadein 0.2
	fadeout 0.2
	fps 10
}

$Sequence "eat" {
	"houndeye_anims\eat.smd"
	activity "ACT_EAT" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "180l" {
	"houndeye_anims\180l.smd"
	activity "ACT_TURN_LEFT" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "180r" {
	"houndeye_anims\180r.smd"
	activity "ACT_TURN_RIGHT" 1
	fadein 0.2
	fadeout 0.2
	fps 30
	loop
}

$Sequence "jumpback" {
	"houndeye_anims\jumpback.smd"
	activity "ACT_LEAP" 1
	{ event 6 6 "" }
	fadein 0.2
	fadeout 0.2
	fps 30
	walkframe 28 LX LY LZ
}

$Sequence "wakefast" {
	"houndeye_anims\wakefast.smd"
	activity "ACT_HOP" 1
	fadein 0.2
	fadeout 0.2
	fps 30
}

$Sequence "whimper" {
	"houndeye_anims\whimper.smd"
	fadein 0.2
	fadeout 0.2
	fps 35
	loop
}

$Sequence "jumpwindow" {
	"houndeye_anims\jumpwindow.smd"
	fadein 0.2
	fadeout 0.2
	fps 28
}

$CollisionJoints "houndeye_physics.smd"
{
	$mass 45
	$inertia 10
	$damping 0.01
	$rotdamping 1.5
	$rootbone "bip01"


	$jointconstrain "Bip01 Spine1" x limit -30 30 0
	$jointconstrain "Bip01 Spine1" y limit -30 30 0
	$jointconstrain "Bip01 Spine1" z limit -30 30 0

	$jointconstrain "Bip01 L Arm1" x limit -30 30 0
	$jointconstrain "Bip01 L Arm1" y limit -40 30 0
	$jointconstrain "Bip01 L Arm1" z limit -40 40 0

	$jointconstrain "Bip01 L Arm2" x limit -30 30 0
	$jointconstrain "Bip01 L Arm2" y limit -15 15 0
	$jointconstrain "Bip01 L Arm2" z limit -60 40 0

	$jointconstrain "Bip01 R Arm1" x limit -30 30 0.2
	$jointconstrain "Bip01 R Arm1" y limit -30 40 0.2
	$jointconstrain "Bip01 R Arm1" z limit -40 40 0

	$jointconstrain "Bip01 L Leg" x limit -30 30 0
	$jointconstrain "Bip01 L Leg" y limit -45 45 0
	$jointconstrain "Bip01 L Leg" z limit -50 30 0

	$jointconstrain "Bip01 L Leg1" x limit -10 10 0
	$jointconstrain "Bip01 L Leg1" y limit 0 0 0
	$jointconstrain "Bip01 L Leg1" z limit -45 20 0

	$jointconstrain "Bip01 R Arm2" x limit -30 30 0
	$jointconstrain "Bip01 R Arm2" y limit -15 15 0
	$jointconstrain "Bip01 R Arm2" z limit -60 40 0
}
