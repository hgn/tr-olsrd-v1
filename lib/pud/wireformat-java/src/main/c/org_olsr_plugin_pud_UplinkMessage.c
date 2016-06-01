/*
 * The olsr.org Optimized Link-State Routing daemon (olsrd)
 *
 * (c) by the OLSR project
 *
 * See our Git repository to find out who worked on this file
 * and thus is a copyright holder on it.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */

#include "org_olsr_plugin_pud_UplinkMessage.h"

#include <OlsrdPudWireFormat/wireFormat.h>

/*
 * Uplink Message Types
 */

/*
 * Class:     org_olsr_plugin_pud_UplinkMessage
 * Method:    getUplinkMessageTypePosition
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_olsr_plugin_pud_UplinkMessage_getUplinkMessageTypePosition
  (JNIEnv * env __attribute__ ((unused)), jclass this __attribute__ ((unused))) {
	return (jint) POSITION;
}

/*
 * Class:     org_olsr_plugin_pud_UplinkMessage
 * Method:    getUplinkMessageTypeClusterLeader
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_olsr_plugin_pud_UplinkMessage_getUplinkMessageTypeClusterLeader
  (JNIEnv * env __attribute__ ((unused)), jclass this __attribute__ ((unused))) {
	return (jint) CLUSTERLEADER;
}

/*
 * Class:     org_olsr_plugin_pud_UplinkMessage
 * Method:    getUplinkMessageHeaderLength
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_olsr_plugin_pud_UplinkMessage_getUplinkMessageHeaderLength
  (JNIEnv * env __attribute__ ((unused)), jclass this __attribute__ ((unused))) {
	return (jint)sizeof(UplinkHeader);
}
