#!/bin/bash
#Script to create network fot the final project demo

echo "Clean any previous network"
cleanNet ovs-bridge

echo "Create basic network"
createOVSBridgedPair veth1 veth2 y ovs-bridge
createOVSBridgedPair veth3 veth4 n ovs-bridge
createOVSBridgedPair veth5 veth6 n ovs-bridge
createOVSBridgedPair veth7 veth8 n ovs-bridge
createOVSBridgedPair veth9 veth10 n ovs-bridge
createOVSBridgedPair veth11 veth12 n ovs-bridge
createOVSBridgedPair veth13 veth14 n ovs-bridge
createOVSBridgedPair veth15 veth16 n ovs-bridge
createOVSBridgedPair veth17 veth18 n ovs-bridge
createOVSBridgedPair veth19 veth20 n ovs-bridge
createOVSBridgedPair veth21 veth22 n ovs-bridge

sudo ifconfig veth1 192.168.1.1 up
sudo ifconfig veth2 192.168.1.2 up
sudo ifconfig veth3 192.168.1.3 up
sudo ifconfig veth4 192.168.1.4 up
sudo ifconfig veth5 192.168.1.5 up
sudo ifconfig veth6 192.168.1.6 up
sudo ifconfig veth7 192.168.1.7 up
sudo ifconfig veth8 192.168.1.8 up
sudo ifconfig veth9 192.168.1.9 up
sudo ifconfig veth10 192.168.1.10 up
sudo ifconfig veth11 192.168.1.11 up
sudo ifconfig veth12 192.168.1.12 up
sudo ifconfig veth13 192.168.1.13 up
sudo ifconfig veth14 192.168.1.14 up
sudo ifconfig veth15 192.168.1.15 up
sudo ifconfig veth16 192.168.1.16 up
sudo ifconfig veth17 192.168.1.17 up
sudo ifconfig veth18 192.168.1.18 up
sudo ifconfig veth19 192.168.1.19 up
sudo ifconfig veth20 192.168.1.20 up
sudo ifconfig veth21 192.168.1.21 up
sudo ifconfig veth22 192.168.1.22 up

