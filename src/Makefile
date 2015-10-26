#
# OMNeT++/OMNEST Makefile for omnetpp-federate
#
# This file was generated with the command:
#  opp_makemake -f --deep -o omnetpp-federate -I../../inetmanet/src/networklayer/arp -I../../inetmanet/src/mobility -I../../inetmanet/src/transport/sctp -I../../inetmanet/src/world -I../../inetmanet/src/transport/contract -I../../inetmanet/src/linklayer/mfcore -I../../inetmanet/src/linklayer/ethernet -I../../inetmanet/src/util -I../../inetmanet/src/networklayer/ted -I../../inetmanet/src/linklayer/ieee80211/mac -I../../inetmanet/src/networklayer/queue -I../../inetmanet/src/networklayer/common -I../../inetmanet/src/networklayer/ipv6 -I../../inetmanet/src/applications/pingapp -I../../inetmanet/src/networklayer/ldp -I../../inetmanet/src/linklayer/radio/errormodel -I../../inetmanet/src/transport/tcp -I../../inetmanet/src/util/headerserializers -I../../inetmanet/src -I../../inetmanet/src/transport/udp -I../../inetmanet/src/networklayer/rsvp_te -I../../inetmanet/src/networklayer/ipv4 -I../../inetmanet/src/networklayer/icmpv6 -I../../inetmanet/src/base -I../../inetmanet/src/networklayer/contract -I../../inetmanet/src/linklayer/ieee80211/mgmt -I../../inetmanet/src/networklayer/manetrouting/base -I../../inetmanet/src/networklayer/mpls -I../../inetmanet/src/linklayer/contract -I../../inetmanet/src/applications/udpapp -I../../inetmanet/src/applications/tcpapp -L../../inetmanet/out/gcc-debug/src/ -lpthread -linet -KINETMANET_PROJ=../../inetmanet
#

# Name of target to be created (-o option)
TARGET = omnetpp-federate$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inetmanet/src/networklayer/arp \
    -I../../inetmanet/src/mobility \
    -I../../inetmanet/src/transport/sctp \
    -I../../inetmanet/src/world \
    -I../../inetmanet/src/transport/contract \
    -I../../inetmanet/src/linklayer/mfcore \
    -I../../inetmanet/src/linklayer/ethernet \
    -I../../inetmanet/src/util \
    -I../../inetmanet/src/networklayer/ted \
    -I../../inetmanet/src/linklayer/ieee80211/mac \
    -I../../inetmanet/src/networklayer/queue \
    -I../../inetmanet/src/networklayer/common \
    -I../../inetmanet/src/networklayer/ipv6 \
    -I../../inetmanet/src/applications/pingapp \
    -I../../inetmanet/src/networklayer/ldp \
    -I../../inetmanet/src/linklayer/radio/errormodel \
    -I../../inetmanet/src/transport/tcp \
    -I../../inetmanet/src/util/headerserializers \
    -I../../inetmanet/src \
    -I../../inetmanet/src/transport/udp \
    -I../../inetmanet/src/networklayer/rsvp_te \
    -I../../inetmanet/src/networklayer/ipv4 \
    -I../../inetmanet/src/networklayer/icmpv6 \
    -I../../inetmanet/src/base \
    -I../../inetmanet/src/networklayer/contract \
    -I../../inetmanet/src/linklayer/ieee80211/mgmt \
    -I../../inetmanet/src/networklayer/manetrouting/base \
    -I../../inetmanet/src/networklayer/mpls \
    -I../../inetmanet/src/linklayer/contract \
    -I../../inetmanet/src/applications/udpapp \
    -I../../inetmanet/src/applications/tcpapp \
    -I. \
    -Ievt \
    -Imgmt \
    -Imob \
    -Imsg \
    -Iutil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inetmanet/out/gcc-debug/src/  -lpthread -linet
LIBS += -Wl,-rpath,`abspath ../../inetmanet/out/gcc-debug/src/`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = /src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/evt/VSimRTIEventScheduler.o \
    $O/mgmt/VSimRTIScenarioManager.o \
    $O/mob/VSimRTIMobility.o \
    $O/msg/BgTrafficApp.o \
    $O/msg/VSimRTIReliableApp.o \
    $O/msg/VSimRTIUnreliableApp.o \
    $O/util/ChannelCMD.o \
    $O/util/ClientServerChannel.o \
    $O/mgmt/VSimRTICommunicationCmd_m.o \
    $O/mgmt/VSimRTIMobilityCmd_m.o \
    $O/msg/VSimRTIAppPacket_m.o

# Message files
MSGFILES = \
    mgmt/VSimRTICommunicationCmd.msg \
    mgmt/VSimRTIMobilityCmd.msg \
    msg/VSimRTIAppPacket.msg

# Other makefile variables (-K)
INETMANET_PROJ=../../inetmanet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f omnetpp-federate omnetpp-federate.exe libomnetpp-federate.so libomnetpp-federate.a libomnetpp-federate.dll libomnetpp-federate.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f evt/*_m.cc evt/*_m.h
	$(Q)-rm -f mgmt/*_m.cc mgmt/*_m.h
	$(Q)-rm -f mob/*_m.cc mob/*_m.h
	$(Q)-rm -f msg/*_m.cc msg/*_m.h
	$(Q)-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc evt/*.cc mgmt/*.cc mob/*.cc msg/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/evt/VSimRTIEventScheduler.o: evt/VSimRTIEventScheduler.cc \
  util/ChannelCMD.h \
  msg/AppProtocolType.h \
  ../../inetmanet/src/base/Coord.h \
  mgmt/VSimRTICommunicationCmd_m.h \
  msg/VSimRTIAppPacket_m.h \
  mgmt/VSimRTIMobilityCmd_m.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  ../../inetmanet/src/base/INETDefs.h \
  util/ClientServerChannel.h \
  evt/VSimRTIEventScheduler.h \
  ../../inetmanet/src/util/FWMath.h
$O/mgmt/VSimRTICommunicationCmd_m.o: mgmt/VSimRTICommunicationCmd_m.cc \
  mgmt/VSimRTICommunicationCmd_m.h \
  ../../inetmanet/src/base/Coord.h \
  msg/AppProtocolType.h \
  ../../inetmanet/src/util/FWMath.h \
  ../../inetmanet/src/base/INETDefs.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h
$O/mgmt/VSimRTIMobilityCmd_m.o: mgmt/VSimRTIMobilityCmd_m.cc \
  ../../inetmanet/src/base/Coord.h \
  ../../inetmanet/src/base/INETDefs.h \
  ../../inetmanet/src/util/FWMath.h \
  mgmt/VSimRTIMobilityCmd_m.h
$O/mgmt/VSimRTIScenarioManager.o: mgmt/VSimRTIScenarioManager.cc \
  msg/VSimRTIAppPacket_m.h \
  evt/VSimRTIEventScheduler.h \
  mgmt/VSimRTIScenarioManager.h \
  msg/VSimRTIUnreliableApp.h \
  ../../inetmanet/src/base/INETDefs.h \
  ../../inetmanet/src/base/ModuleAccess.h \
  ../../inetmanet/src/world/ChannelControl.h \
  mob/VSimRTIMobility.h \
  msg/AppProtocolType.h \
  util/ChannelCMD.h \
  msg/BgTrafficApp.h \
  mgmt/VSimRTICommunicationCmd_m.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  ../../inetmanet/src/transport/contract/TCPSocket.h \
  ../../inetmanet/src/base/BasicModule.h \
  mgmt/VSimRTIMobilityCmd_m.h \
  ../../inetmanet/src/applications/udpapp/UDPAppBase.h \
  ../../inetmanet/src/base/NotifierConsts.h \
  util/ClientServerChannel.h \
  ../../inetmanet/src/base/NotificationBoard.h \
  ../../inetmanet/src/util/FWMath.h \
  ../../inetmanet/src/linklayer/mfcore/AirFrame_m.h \
  ../../inetmanet/src/applications/tcpapp/TCPGenericCliAppBase.h \
  ../../inetmanet/src/networklayer/contract/IPvXAddress.h \
  ../../inetmanet/src/networklayer/contract/IPv6Address.h \
  ../../inetmanet/src/mobility/BasicMobility.h \
  ../../inetmanet/src/base/INotifiable.h \
  msg/VSimRTIReliableApp.h \
  ../../inetmanet/src/base/Coord.h \
  ../../inetmanet/src/linklayer/mfcore/ModulationType.h \
  ../../inetmanet/src/transport/contract/TCPCommand_m.h
$O/mob/VSimRTIMobility.o: mob/VSimRTIMobility.cc \
  ../../inetmanet/src/linklayer/mfcore/ModulationType.h \
  ../../inetmanet/src/base/INotifiable.h \
  ../../inetmanet/src/base/Coord.h \
  ../../inetmanet/src/mobility/BasicMobility.h \
  ../../inetmanet/src/base/NotificationBoard.h \
  ../../inetmanet/src/base/NotifierConsts.h \
  ../../inetmanet/src/linklayer/mfcore/AirFrame_m.h \
  ../../inetmanet/src/util/FWMath.h \
  ../../inetmanet/src/base/BasicModule.h \
  mob/VSimRTIMobility.h \
  ../../inetmanet/src/world/ChannelControl.h \
  ../../inetmanet/src/base/ModuleAccess.h \
  ../../inetmanet/src/base/INETDefs.h
$O/msg/BgTrafficApp.o: msg/BgTrafficApp.cc \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  ../../inetmanet/src/applications/udpapp/UDPAppBase.h \
  ../../inetmanet/src/networklayer/contract/IPvXAddress.h \
  ../../inetmanet/src/networklayer/contract/IPv6Address.h \
  msg/BgTrafficApp.h \
  ../../inetmanet/src/base/INETDefs.h
$O/msg/VSimRTIAppPacket_m.o: msg/VSimRTIAppPacket_m.cc \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  msg/VSimRTIAppPacket_m.h \
  ../../inetmanet/src/base/INETDefs.h
$O/msg/VSimRTIReliableApp.o: msg/VSimRTIReliableApp.cc \
  ../../inetmanet/src/base/INETDefs.h \
  ../../inetmanet/src/transport/contract/TCPCommand_m.h \
  msg/VSimRTIReliableApp.h \
  ../../inetmanet/src/networklayer/contract/IPvXAddress.h \
  ../../inetmanet/src/applications/tcpapp/TCPGenericCliAppBase.h \
  ../../inetmanet/src/networklayer/contract/IPv6Address.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  ../../inetmanet/src/transport/contract/TCPSocket.h
$O/msg/VSimRTIUnreliableApp.o: msg/VSimRTIUnreliableApp.cc \
  msg/VSimRTIUnreliableApp.h \
  ../../inetmanet/src/base/INETDefs.h \
  msg/VSimRTIAppPacket_m.h \
  ../../inetmanet/src/networklayer/contract/IPv6Address.h \
  ../../inetmanet/src/networklayer/contract/IPvXAddress.h \
  ../../inetmanet/src/transport/contract/UDPControlInfo_m.h \
  ../../inetmanet/src/applications/udpapp/UDPAppBase.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h
$O/util/ChannelCMD.o: util/ChannelCMD.cc \
  util/ChannelCMD.h
$O/util/ClientServerChannel.o: util/ClientServerChannel.cc \
  util/ChannelCMD.h \
  ../../inetmanet/src/base/INETDefs.h \
  ../../inetmanet/src/base/Coord.h \
  ../../inetmanet/src/networklayer/contract/IPAddress.h \
  ../../inetmanet/src/util/FWMath.h \
  util/ClientServerChannel.h
