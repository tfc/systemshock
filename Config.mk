PROJECT=systemshock

OUT=$PROJECT
TOOLCHAIN=local
UNAME_S := $(shell uname -s)
INCPATH=/usr/include

LIBS=\
	-lSDL2\
	-lSDL2_mixer

CFLAGS=\
	-D_REENTRANT\
	-DSVGA_SUPPORT\
	-Isrc/Libraries/2D/Source\
	-Isrc/Libraries/2D/Source/Clip\
	-Isrc/Libraries/2D/Source/Flat8\
	-Isrc/Libraries/2D/Source/GR\
	-Isrc/Libraries/2D/Source/Gen\
	-Isrc/Libraries/2D/Source/RSD\
	-Isrc/Libraries/3D/Source\
	-Isrc/Libraries/AFILE/Source\
	-Isrc/Libraries/DSTRUCT/Source\
	-Isrc/Libraries/EDMS/Source\
	-Isrc/Libraries/FIXPP/Source\
	-Isrc/Libraries/INPUT/Source\
	-Isrc/Libraries/PALETTE/Source\
	-Isrc/Libraries/RES/Source\
	-Isrc/Libraries/UI/Source\
	-Isrc/Libraries/RND/Source\
	-Isrc/Libraries/VOX/Source\
	-Isrc/Libraries/FIX/Source\
	-Isrc/Libraries/SND/Source\
	-Isrc/Libraries/H\
	-Isrc/Libraries/LG/Source\
	-Isrc/Libraries/LG/Source/LOG/src\
	-Isrc/Libraries/adlmidi/include\
	-Isrc/GameSrc/Headers\
	-Isrc/MacSrc\
	-Isrc/MusicSrc\
	-Isrc/stubs\
	-isystem $(INCPATH)/libxml2\
	-isystem $(INCPATH)/SDL2\
	-O2\
	-pedantic-errors\
	-std=c99\
	-Wall\
	-Werror\
	-Wextra

SRCS=\
	src/MacSrc/Stub.c\
	src/MacSrc/ShockBitmap.c\
	src/MacSrc/MoviePlay.c\
	src/MacSrc/InitMac.c\
	src/MacSrc/Shock.c\
	src/MacSrc/Prefs.c\
	src/MacSrc/MacTune.c\
	src/MacSrc/SDLSound.c\
	src/MacSrc/Modding.c\
	src/MacSrc/OpenGL.cc\
	src/MacSrc/Xmi.c\
	src/MusicSrc/MusicDevice.c\
	src/GameSrc/ai.c\
	src/GameSrc/airupt.c\
	src/GameSrc/amap.c\
	src/GameSrc/amaploop.c\
	src/GameSrc/ammomfd.c\
	src/GameSrc/anim.c\
	src/GameSrc/audiolog.c\
	src/GameSrc/automap.c\
	src/GameSrc/bark.c\
	src/GameSrc/biohelp.c\
	src/GameSrc/cardmfd.c\
	src/GameSrc/citres.c\
	src/GameSrc/combat.c\
	src/GameSrc/cone.c\
	src/GameSrc/criterr.c\
	src/GameSrc/cyber.c\
	src/GameSrc/cybermfd.c\
	src/GameSrc/cybmem.c\
	src/GameSrc/cybrnd.c\
	src/GameSrc/cutsloop.c\
	src/GameSrc/damage.c\
	src/GameSrc/digifx.c\
	src/GameSrc/drugs.c\
	src/GameSrc/effect.c\
	src/GameSrc/email.c\
	src/GameSrc/faceobj.c\
	src/GameSrc/fixtrmfd.c\
	src/GameSrc/frcamera.c\
	src/GameSrc/frclip.c\
	src/GameSrc/frcompil.c\
	src/GameSrc/frmain.c\
	src/GameSrc/frobj.c\
	src/GameSrc/froslew.c\
	src/GameSrc/frpipe.c\
	src/GameSrc/frpts.c\
	src/GameSrc/frsetup.c\
	src/GameSrc/frtables.c\
	src/GameSrc/frterr.c\
	src/GameSrc/frutil.c\
	src/GameSrc/FrUtils.c\
	src/GameSrc/fullamap.c\
	src/GameSrc/fullscrn.c\
	src/GameSrc/gameloop.c\
	src/GameSrc/gameobj.c\
	src/GameSrc/gamesort.c\
	src/GameSrc/gamestrn.c\
	src/GameSrc/gamesys.c\
	src/GameSrc/gametime.c\
	src/GameSrc/gamewrap.c\
	src/GameSrc/gearmfd.c\
	src/GameSrc/gr2ss.c\
	src/GameSrc/grenades.c\
	src/GameSrc/hand.c\
	src/GameSrc/hflip.c\
	src/GameSrc/hkeyfunc.c\
	src/GameSrc/hud.c\
	src/GameSrc/hudobj.c\
	src/GameSrc/init.c\
	src/GameSrc/input.c\
	src/GameSrc/invent.c\
	src/GameSrc/leanmetr.c\
	src/GameSrc/mainloop.c\
	src/GameSrc/map.c\
	src/GameSrc/mfdfunc.c\
	src/GameSrc/mfdgadg.c\
	src/GameSrc/mfdgames.c\
	src/GameSrc/mfdgump.c\
	src/GameSrc/mfdpanel.c\
	src/GameSrc/minimax.c\
	src/GameSrc/mlimbs.c\
	src/GameSrc/movekeys.c\
	src/GameSrc/musicai.c\
	src/GameSrc/newai.c\
	src/GameSrc/newmfd.c\
	src/GameSrc/objapp.c\
	src/GameSrc/objects.c\
	src/GameSrc/objload.c\
	src/GameSrc/objprop.c\
	src/GameSrc/objsim.c\
	src/GameSrc/objuse.c\
	src/GameSrc/olh.c\
	src/GameSrc/olhscan.c\
	src/GameSrc/palfx.c\
	src/GameSrc/pathfind.c\
	src/GameSrc/physics.c\
	src/GameSrc/player.c\
	src/GameSrc/plotware.c\
	src/GameSrc/popups.c\
	src/GameSrc/render.c\
	src/GameSrc/rendtool.c\
	src/GameSrc/saveload.c\
	src/GameSrc/schedule.c\
	src/GameSrc/screen.c\
	src/GameSrc/setup.c\
	src/GameSrc/shodan.c\
	src/GameSrc/sideicon.c\
	src/GameSrc/sndcall.c\
	src/GameSrc/star.c\
	src/GameSrc/statics.c\
	src/GameSrc/status.c\
	src/GameSrc/target.c\
	src/GameSrc/textmaps.c\
	src/GameSrc/tfdirect.c\
	src/GameSrc/tfutil.c\
	src/GameSrc/tools.c\
	src/GameSrc/trigger.c\
	src/GameSrc/view360.c\
	src/GameSrc/viewhelp.c\
	src/GameSrc/vitals.c\
	src/GameSrc/vmail.c\
	src/GameSrc/wares.c\
	src/GameSrc/weapons.c\
	src/GameSrc/wrapper.c\
	src/GameSrc/gamerend.c\
	src/GameSrc/mouselook.c

OBJS=$(patsubst %.c, %.o, $(SRCS))

2D_SRCS=\
	src/Libraries/2D/Source/bit.c\
	src/Libraries/2D/Source/bitmap.c\
	src/Libraries/2D/Source/blend.c\
	src/Libraries/2D/Source/buffer.c\
	src/Libraries/2D/Source/canvas.c\
	src/Libraries/2D/Source/chain.c\
	src/Libraries/2D/Source/ChnFuncs.c\
	src/Libraries/2D/Source/chrsiz.c\
	src/Libraries/2D/Source/chrwid.c\
	src/Libraries/2D/Source/close.c\
	src/Libraries/2D/Source/cnvtab.c\
	src/Libraries/2D/Source/context.c\
	src/Libraries/2D/Source/detect.c\
	src/Libraries/2D/Source/devtab.c\
	src/Libraries/2D/Source/fcntab.c\
	src/Libraries/2D/Source/init.c\
	src/Libraries/2D/Source/invtab.c\
	src/Libraries/2D/Source/lintab.c\
	src/Libraries/2D/Source/mode.c\
	src/Libraries/2D/Source/pal.c\
	src/Libraries/2D/Source/permap.c\
	src/Libraries/2D/Source/persetup.c\
	src/Libraries/2D/Source/pertol.c\
	src/Libraries/2D/Source/pixfill.c\
	src/Libraries/2D/Source/rgb.c\
	src/Libraries/2D/Source/screen.c\
	src/Libraries/2D/Source/sscrn.c\
	src/Libraries/2D/Source/StateStk.c\
	src/Libraries/2D/Source/strnsiz.c\
	src/Libraries/2D/Source/strscl.c\
	src/Libraries/2D/Source/strsiz.c\
	src/Libraries/2D/Source/struscl.c\
	src/Libraries/2D/Source/strwid.c\
	src/Libraries/2D/Source/strwrap.c\
	src/Libraries/2D/Source/svgainit.c\
	src/Libraries/2D/Source/tempbm.c\
	src/Libraries/2D/Source/temptm.c\
	src/Libraries/2D/Source/tlucdat.c\
	src/Libraries/2D/Source/tluctab.c\
	src/Libraries/2D/Source/valloc.c\
	src/Libraries/2D/Source/vtab.c\
	src/Libraries/2D/Source/wirpoly.c\
	src/Libraries/2D/Source/MacDev.c\
	src/Libraries/2D/Source/GR/grilin.c\
	src/Libraries/2D/Source/GR/grmalloc.c\
	src/Libraries/2D/Source/GR/grnull.c\
	src/Libraries/2D/Source/GR/gruilin.c\
	src/Libraries/2D/Source/GR/grd.c\
	src/Libraries/2D/Source/Gen/gendisk.c\
	src/Libraries/2D/Source/Gen/gentm.c\
	src/Libraries/2D/Source/Gen/genwclin.c\
	src/Libraries/2D/Source/Gen/genuclin.c\
	src/Libraries/2D/Source/Gen/genrsd8.c\
	src/Libraries/2D/Source/Gen/genuvlin.c\
	src/Libraries/2D/Source/Gen/genhlin.c\
	src/Libraries/2D/Source/Gen/genrsdbm.c\
	src/Libraries/2D/Source/Gen/genuchr.c\
	src/Libraries/2D/Source/Gen/genvlin.c\
	src/Libraries/2D/Source/Gen/genvcply.c\
	src/Libraries/2D/Source/Gen/genstr.c\
	src/Libraries/2D/Source/Gen/genclin.c\
	src/Libraries/2D/Source/Gen/genfl8c.c\
	src/Libraries/2D/Source/Gen/genrsdtm.c\
	src/Libraries/2D/Source/Gen/genel.c\
	src/Libraries/2D/Source/Gen/genchr.c\
	src/Libraries/2D/Source/Gen/genwlin.c\
	src/Libraries/2D/Source/Gen/genpix.c\
	src/Libraries/2D/Source/Gen/genuhlin.c\
	src/Libraries/2D/Source/Gen/genustr.c\
	src/Libraries/2D/Source/Gen/genhfl8.c\
	src/Libraries/2D/Source/Gen/genov.c\
	src/Libraries/2D/Source/Gen/gente.c\
	src/Libraries/2D/Source/Gen/genlin.c\
	src/Libraries/2D/Source/Gen/gencwlin.c\
	src/Libraries/2D/Source/Gen/genchfl8.c\
	src/Libraries/2D/Source/Gen/gengfl8.c\
	src/Libraries/2D/Source/Gen/genbox.c\
	src/Libraries/2D/Source/Gen/genmono.c\
	src/Libraries/2D/Source/Gen/genulin.c\
	src/Libraries/2D/Source/Gen/gencnv.c\
	src/Libraries/2D/Source/Gen/genuslin.c\
	src/Libraries/2D/Source/Gen/genvpoly.c\
	src/Libraries/2D/Source/Gen/general.c\
	src/Libraries/2D/Source/Gen/gencirc.c\
	src/Libraries/2D/Source/Gen/genfl8.c\
	src/Libraries/2D/Source/Gen/genf24.c\
	src/Libraries/2D/Source/Gen/gentl8.c\
	src/Libraries/2D/Source/Gen/genrect.c\
	src/Libraries/2D/Source/Gen/genvrect.c\
	src/Libraries/2D/Source/Gen/genslin.c\
	src/Libraries/2D/Source/Flat8/fl8vlin.c\
	src/Libraries/2D/Source/Flat8/FL8OPL.c\
	src/Libraries/2D/Source/Flat8/fl8cply.c\
	src/Libraries/2D/Source/Flat8/fl8clin.c\
	src/Libraries/2D/Source/Flat8/fl8lp.c\
	src/Libraries/2D/Source/Flat8/fl8lf.c\
	src/Libraries/2D/Source/Flat8/fl8ll.c\
	src/Libraries/2D/Source/Flat8/fl8s.c\
	src/Libraries/2D/Source/Flat8/fl8hfl8.c\
	src/Libraries/2D/Source/Flat8/fl8pnt.c\
	src/Libraries/2D/Source/Flat8/fl8fl8m.c\
	src/Libraries/2D/Source/Flat8/fl8ltp.c\
	src/Libraries/2D/Source/Flat8/fl8fl8c.c\
	src/Libraries/2D/Source/Flat8/fl8w.c\
	src/Libraries/2D/Source/Flat8/fl8cop.c\
	src/Libraries/2D/Source/Flat8/fl8wlin.c\
	src/Libraries/2D/Source/Flat8/fl8lw.c\
	src/Libraries/2D/Source/Flat8/fl8pix.c\
	src/Libraries/2D/Source/Flat8/fl8row.c\
	src/Libraries/2D/Source/Flat8/fl8ft.c\
	src/Libraries/2D/Source/Flat8/fl8rsd8.c\
	src/Libraries/2D/Source/Flat8/Fl8F.c\
	src/Libraries/2D/Source/Flat8/fl8bldbl.c\
	src/Libraries/2D/Source/Flat8/fl8clear.c\
	src/Libraries/2D/Source/Flat8/fl8p.c\
	src/Libraries/2D/Source/Flat8/fl8wclin.c\
	src/Libraries/2D/Source/Flat8/fl8hlin.c\
	src/Libraries/2D/Source/Flat8/fl8bl.c\
	src/Libraries/2D/Source/Flat8/fl8gpix.c\
	src/Libraries/2D/Source/Flat8/fl8ns.c\
	src/Libraries/2D/Source/Flat8/fl8lnop.c\
	src/Libraries/2D/Source/Flat8/fl8dbl.c\
	src/Libraries/2D/Source/Flat8/fl8cnv.c\
	src/Libraries/2D/Source/Flat8/fl8slin.c\
	src/Libraries/2D/Source/Flat8/fl8lin.c\
	src/Libraries/2D/Source/Flat8/fl8tsmap.c\
	src/Libraries/2D/Source/Flat8/fl8ply.c\
	src/Libraries/2D/Source/Flat8/fl8sply.c\
	src/Libraries/2D/Source/Flat8/fl8rect.c\
	src/Libraries/2D/Source/Flat8/fl8gfl8.c\
	src/Libraries/2D/Source/Flat8/fl8tl8.c\
	src/Libraries/2D/Source/Flat8/fl8p24.c\
	src/Libraries/2D/Source/Flat8/fl8g24.c\
	src/Libraries/2D/Source/Flat8/fl8ctp.c\
	src/Libraries/2D/Source/Flat8/fl8mscl.c\
	src/Libraries/2D/Source/Flat8/fl8lop.c\
	src/Libraries/2D/Source/Flat8/fl8ntrp2.c\
	src/Libraries/2D/Source/Flat8/fl8sub.c\
	src/Libraries/2D/Source/Flat8/fl8tpl.c\
	src/Libraries/2D/Source/Flat8/fl8nl.c\
	src/Libraries/2D/Source/Flat8/fl8fltr2.c\
	src/Libraries/2D/Source/Flat8/fl8mono.c\
	src/Libraries/2D/Source/Flat8/fl8fl8.c\
	src/Libraries/2D/Source/Flat8/fl8chfl8.c\
	src/Libraries/2D/Source/Clip/clpply.c\
	src/Libraries/2D/Source/Clip/clplin.c\
	src/Libraries/2D/Source/Clip/clpclin.c\
	src/Libraries/2D/Source/Clip/clpltab.c\
	src/Libraries/2D/Source/Clip/clpf24.c\
	src/Libraries/2D/Source/Clip/clplin2.c\
	src/Libraries/2D/Source/Clip/clppoly.c\
	src/Libraries/2D/Source/Clip/clpslin.c\
	src/Libraries/2D/Source/Clip/clprect.c\
	src/Libraries/2D/Source/Clip/clpmono.c\
	src/Libraries/2D/Source/RSD/rsdcvt.c\
	src/Libraries/2D/Source/RSD/RSDUnpack.c

2D_OBJS=$(patsubst %.c, %.o, $(2D_SRCS))

3D_SRCS=\
	src/Libraries/3D/Source/alloc.c\
	src/Libraries/3D/Source/Bitmap.c\
	src/Libraries/3D/Source/clip.c\
	src/Libraries/3D/Source/detail.c\
	src/Libraries/3D/Source/fov.c\
	src/Libraries/3D/Source/GlobalV.c\
	src/Libraries/3D/Source/instance.c\
	src/Libraries/3D/Source/interp.c\
	src/Libraries/3D/Source/light.c\
	src/Libraries/3D/Source/matrix.c\
	src/Libraries/3D/Source/points.c\
	src/Libraries/3D/Source/polygon.c\
	src/Libraries/3D/Source/slew.c\
	src/Libraries/3D/Source/tmap.c\
	src/Libraries/3D/Source/vector.c

3D_OBJS=$(patsubst %.c, %.o, $(3D_SRCS))

AFILE_SRCS=\
	src/Libraries/AFILE/Source/afile.c\
	src/Libraries/AFILE/Source/amov.c\
	src/Libraries/AFILE/Source/compose.c\
	src/Libraries/AFILE/Source/movie.c

AFILE_OBJS=$(patsubst %.c, %.o, $(AFILE_SRCS))

AFILE_CPP_SRCS=\
	src/Libraries/AFILE/Source/draw4x4.cpp\
	src/Libraries/AFILE/Source/huffde.cpp

AFILE_CPP_OBJS=$(patsubst %.cpp, %.o, $(AFILE_CPP_SRCS))

DSTRUCT_SRCS=\
	src/Libraries/DSTRUCT/Source/array.c\
	src/Libraries/DSTRUCT/Source/hash.c\
	src/Libraries/DSTRUCT/Source/llist.c\
	src/Libraries/DSTRUCT/Source/lllist.c\
	src/Libraries/DSTRUCT/Source/pqueue.c\
	src/Libraries/DSTRUCT/Source/rect.c

DSTRUCT_OBJS=$(patsubst %.c, %.o, $(DSTRUCT_SRCS))

FIX_SRCS=\
	src/Libraries/FIX/Source/f_exp.c\
	src/Libraries/FIX/Source/fix.c\
	src/Libraries/FIX/Source/fix_pow.c\
	src/Libraries/FIX/Source/fix_sqrt.c\
	src/Libraries/FIX/Source/MakeTables.c

FIX_OBJS=$(patsubst %.c, %.o, $(FIX_SRCS))

FIX_CPP_SRCS=src/Libraries/FIXPP/Source/fixpp.cpp
FIX_CPP_OBJS=$(patsubst %.cpp, %.o, $(FIX_SRCS))

INPUT_SRCS=\
	src/Libraries/INPUT/Source/kbcook.c\
	src/Libraries/INPUT/Source/mouse.c\
	src/Libraries/INPUT/Source/sdl_events.c

INPUT_OBJS=$(patsubst %.c, %.o, $(INPUT_SRCS))

LG_SRCS=\
	src/Libraries/LG/Source/LOG/src/log.c\
	src/Libraries/LG/Source/dbg.c\
	src/Libraries/LG/Source/lgsprntf.c\
	src/Libraries/LG/Source/memall.c\
	src/Libraries/LG/Source/stack.c\
	src/Libraries/LG/Source/tmpalloc.c\
	src/stubs/Carbon/carbon_stubs.c

LG_OBJS=$(patsubst %.c, %.o, $(LG_SRCS))

PALETTE_SRCS=src/Libraries/PALETTE/Source/palette.c
PALETTE_OBJS=$(patsubst %.c, %.o, $(PALETTE_SRCS))

RES_SRCS=\
	src/Libraries/RES/Source/caseless.c\
	src/Libraries/RES/Source/lzw.c\
	src/Libraries/RES/Source/refacc.c\
	src/Libraries/RES/Source/resacc.c\
	src/Libraries/RES/Source/resbuild.c\
	src/Libraries/RES/Source/res.c\
	src/Libraries/RES/Source/resfile.c\
	src/Libraries/RES/Source/resload.c\
	src/Libraries/RES/Source/resmake.c\
	src/Libraries/RES/Source/restypes.c

RES_OBJS=$(patsubst %.c, %.o, $(RES_SRCS))

RND_SRCS=src/Libraries/2D/Source/RND/rnd.c
RND_OBJS=$(patsubst %.c, %.o, $(RND_SRCS))

SND_SRCS=\
	src/Libraries/SND/Source/dig_init.c\
	src/Libraries/SND/Source/dig_ops.c\
	src/Libraries/SND/Source/master.c\
	src/Libraries/SND/Source/mid_init.c\
	src/Libraries/SND/Source/mid_ops.c\
	src/Libraries/SND/Source/snd_util.c

SND_OBJS=$(patsubst %.c, %.o, $(SND_SRCS))

UI_SRCS=\
	src/Libraries/UI/Source/butarray.c\
	src/Libraries/UI/Source/curdrw.c\
	src/Libraries/UI/Source/cursors.c\
	src/Libraries/UI/Source/event.c\
	src/Libraries/UI/Source/gadgets.c\
	src/Libraries/UI/Source/hotkey.c\
	src/Libraries/UI/Source/menu.c\
	src/Libraries/UI/Source/monofont.c\
	src/Libraries/UI/Source/plain.c\
	src/Libraries/UI/Source/pushbutt.c\
	src/Libraries/UI/Source/qboxgadg.c\
	src/Libraries/UI/Source/realfont.c\
	src/Libraries/UI/Source/region.c\
	src/Libraries/UI/Source/slab.c\
	src/Libraries/UI/Source/slider.c\
	src/Libraries/UI/Source/textgadg.c\
	src/Libraries/UI/Source/texttool.c\
	src/Libraries/UI/Source/tngbarry.c\
	src/Libraries/UI/Source/tng.c\
	src/Libraries/UI/Source/tngfont.c\
	src/Libraries/UI/Source/tngmenu.c\
	src/Libraries/UI/Source/tngplain.c\
	src/Libraries/UI/Source/tngpushb.c\
	src/Libraries/UI/Source/tngqbox.c\
	src/Libraries/UI/Source/tngslidr.c\
	src/Libraries/UI/Source/tngtemp.c\
	src/Libraries/UI/Source/tngtextg.c\
	src/Libraries/UI/Source/uires.c\
	src/Libraries/UI/Source/vmouse.c

UI_OBJS=$(patsubst %.c, %.o, $(UI_SRCS))

EDMS_SRCS=\
	src/Libraries/EDMS/Source/interfac.cc\
	src/Libraries/EDMS/Source/collide.cc\
	src/Libraries/EDMS/Source/intrsect.cc\
	src/Libraries/EDMS/Source/globals.cc\
	src/Libraries/EDMS/Source/phy_tool.cc\
	src/Libraries/EDMS/Source/soliton.cc\
	src/Libraries/EDMS/Source/MODELS/robot.cc\
	src/Libraries/EDMS/Source/MODELS/pelface.cc\
	src/Libraries/EDMS/Source/MODELS/ftl.cc\
	src/Libraries/EDMS/Source/MODELS/pelvis.cc\
	src/Libraries/EDMS/Source/MODELS/d_frame.cc\
	src/Libraries/EDMS/Source/MODELS/d_f_face.cc\
	src/Libraries/EDMS/Source/MODELS/ftlface.cc\
	src/Libraries/EDMS/Source/MODELS/d_f_2.cc

EDMS_OBJS=$(patsubst %.cc, %.o, $(EDMS_SRCS))

VOX_SRCS=\
	src/Libraries/VOX/Source/vox2d.c\
	src/Libraries/VOX/Source/vox3d.c\
	src/Libraries/VOX/Source/voxinit.c

VOX_OBJS=$(patsubst %.c, %.o, $(VOX_SRCS))
