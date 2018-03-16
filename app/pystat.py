#!/bin/env python

import sys
import getopt
import os

from batchq import computeNode
from batchq import batchq


# globals
nNodes = 43
FreeUseNodes = [1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19]
physicsNodes = [40,41,42,43]
allNodes = range(1,44)
goodQueueNames = ['defaultfaculty','physics']
nodes = {} # a dictionary with nodes in the range node1 - node43 excluding 20 - 39
inOptions = {
  'v' : ['summary','Short summary of available slots'],
  'h' : ['help','print help statement'],
  'u' : ['users','show all active users on queue'],
}

facultyUsers = ['csnow','fdayan', 'gjia', 'munsky', 'robj', 'sguzik', 'sjathar', 'vkg']
physicsUsers = ['dcherdack','erezra','jschwehr','mhogan','tcampbell']
uGradsUsers = ['adday','akowals','alia','apaudel','atfox','chenshen','chunter7495','cquiroz','dmangesh','edelen','esailaja','hansentm','hhatoum','hpc08','hpc17','jhardin','jpcavicc','kkiwimag','lfhartje','llweber','lucasj','lyfang','mandor','marklunt','mpmay','mtanha','sansade','scompton','sdufrane','supracol','thaila','thuber','tjdecker','trungdo','ukasha','virajrp','wsraymon','yjung','yxshao','zrfox']
staffUsers = ['shaila','jeff']




def GetOutputFromCommand(command,splitBy = ''):
  out = os.popen(command).read()
  if out.find('error') != -1:
    if len(splitBy) > 0:
      return []
    else:
      return ''
  if len(splitBy) > 0:
    out = out.split(splitBy)
  return out

def InitNodes():
  for i in range(1,nNodes+1):
    if not (i in FreeUseNodes or i in physicsNodes):
      continue
    if i in FreeUseNodes:
        opportunistic = 1
    nodeName = 'node'+str(i)
    node = computeNode(nodeName, opportunistic)
    if i in physicsNodes:
      node.SetNCores(16)
    nodes[nodeName] = node
  currentJobs = GetOutputFromCommand('qstat -f','\n')
  for line in currentJobs:
    if line.find('.q@') == -1:
      continue
    index = line.find('@')
    while line[index] != ' ':
	index += 1	
    index -= 1
    hostName = line[line.find('@')+1:index+2].strip()
    if hostName not in nodes.keys():
      #print hostName+' not found. continuing'
      continue
    queueName = line[0:line.find('.q')].strip()
    if queueName not in goodQueueNames:
      #print queueName+' is not a good queue'
      continue
    #jobsAndSlots = line.split('BIP   ')[1].split('.')[0]
    jobsAndSlots = line.split('BIP  ')[1]
    if jobsAndSlots.find('.') != -1:
      jobsAndSlots.split('.')[0]
      jobsAndSlots = jobsAndSlots[0:len(jobsAndSlots)-5].strip().split('/')
      jobsAndSlots[2] = jobsAndSlots[2][0:3].strip()
    elif jobsAndSlots.find('-NA-') != -1:
      jobsAndSlots.split('-NA-')[0]
      jobsAndSlots = jobsAndSlots[0:len(jobsAndSlots)-5].strip().split('/')
      jobsAndSlots[2] = jobsAndSlots[2][0:3].strip()
    else:
      jobsAndSlots = ['0','0','0']
    state = line.split('lx-amd64')[1].strip()
    reserved = jobsAndSlots[0]
    jobs = jobsAndSlots[1]
    slots = jobsAndSlots[2]
    #print hostName
    #print 'slots = '+str(slots)
    #print 'jobs = '+str(jobs)
    node = nodes[hostName] 
    node.SetNJobs(jobs);
    node.SetNCores(slots)
    node.SetQueueName(queueName)
    if len(state) > 0:
      if len(node.GetState()) == 0:
	node.SetState(state)
      if state not in node.GetState():
	node.SetState(node.GetState()+','+state)

def GetJobsFromUser(username,qstatOutput=[]):
  if len(qstatOutput) == 0:
    qstatOutput = GetOutputFromCommand('qstat -u %s'%(username),'\n')
  userJobs = {'queued' : 0,'slots' : 0,'errors' : 0, 'suspended' : 0}
  for line in qstatOutput:
    if line.find(username) == -1:
      continue
    skipJob = True
    for node in allNodes:
      nodeName = 'node%d '%(node)
      if nodeName in line:
	skipJob = False
	break
    if line.find(' qw ') == -1 and line.find(' S ') == -1 and line.find(' s ') == -1 and skipJob:
      continue
    if line.find(' r ') != -1:
      splitLine = line.split('                       ')
      userJobs['slots']  += int(splitLine[len(splitLine)-1])
    if line.find(' qw ') != -1:
      userJobs['queued'] += 1
    if line.find(' Eqw ') != -1 or line.find(' Er ') != -1 or line.find(' E ') != -1:
      userJobs['errors'] += 1
    if line.find(' S ') != -1 or line.find(' s ') != -1:
      splitLine = line.split('                       ')
      userJobs['suspended']  += int(splitLine[len(splitLine)-1])

  return userJobs

def GetLongOption(shortOption):
  if shortOption[0] == '-':
    shortOption = shortOption[1:len(shortOption)]
  longOption = inOptions[shortOption][0]
  if longOption.find('=') != -1:
    longOption = longOption[0:len(longOption)-1]
  return '--'+longOption

def PrintNodeInfo(nodeName):
  if nodeName not in nodes.keys():
    return
  if len(nodes[nodeName].GetState()) > 0:
    print '| %6s |  --   |'%(nodeName)
    return
  print '| %6s |  %2d   |'%(nodeName,nodes[nodeName].GetNFreeCores())


def main(argv):

    this_node = computeNode

    facultyUsers = ['csnow', 'fdayan', 'gjia', 'munsky', 'robj', 'sguzik',
                    'sjathar', 'vkg']
    physicsUsers = ['dcherdack', 'erezra', 'jschwehr', 'mhogan', 'tcampbell']
    uGradsUsers = ['adday', 'akowals', 'alia', 'apaudel', 'atfox', 'chenshen',
                   'chunter7495', 'cquiroz', 'dmangesh', 'edelen', 'esailaja',
                   'hansentm', 'hhatoum', 'hpc08', 'hpc17', 'jhardin',
                   'jpcavicc', 'kkiwimag', 'lfhartje', 'llweber', 'lucasj',
                   'lyfang', 'mandor', 'marklunt', 'mpmay', 'mtanha',
                   'sansade', 'scompton', 'sdufrane', 'supracol', 'thaila',
                   'thuber', 'tjdecker', 'trungdo', 'ukasha', 'virajrp',
                   'wsraymon', 'yjung', 'yxshao', 'zrfox']
    staffUsers = ['shaila', 'jeff']

    bq = batchq('enshpc')
    bq.addusers('facultyUsers', facultyUsers)
    bq.addusers('physicsUsers', physicsUsers)
    bq.addusers('staffUsers', staffUsers)

    runHost = os.popen('hostname').read().split('\n')[0]
    if runHost.find('node') != -1:
        print 'ERROR: %s is not a suitable batch submission node'%(runHost)
        sys.exit(2)
    shortArgsList = []
    longArgsList = []
    argDescriptionList = []
    for key in inOptions.keys():
        sublist = inOptions[key]
        shortArgsList.insert(0,key)
        longArgsList.insert(0,sublist[0])
        argDescriptionList.insert(0,sublist[1])
    shortArgs = ''.join(shortArgsList)
    helpstatement = 'qstat.py [OPTIONS]\n'
    for ele in range(0,len(shortArgsList)):
        helpstatement=helpstatement+'-%s/--%s == %s \n'%(shortArgsList[ele][0:1],longArgsList[ele][0:len(longArgsList[ele])],argDescriptionList[ele])
    try:
        opts, args = getopt.getopt(argv,shortArgs,longArgsList)
    except getopt.GetoptError:
        print 'Option ERROR'
        print 'Printing help statement'
        print helpstatement
        sys.exit(2)

    #print 'init nodes'
    InitNodes()
    #print 'done'
    showHosts = False
    showUsers = False
    for opt, arg in opts:
        if opt in ('-h',GetLongOption('-h')):
            print helpstatement
            sys.exit(2)
        elif opt in ('-v',GetLongOption('-v')):
            showHosts = True
        elif opt in ('-u',GetLongOption('-u')):
            showUsers = True
        else:
            print 'ERROR: Parameter not known: '+str(opt)
            print helpstatement
            sys.exit(2) 
    availableSlots = 0
    for aNode in nodes.values():
        if len(aNode.GetState()) > 0:
            continue
        availableSlots += aNode.GetNFreeCores()
    if showHosts:
        print '=================='
        print '|                |    Open |'
        print '|    Name    | Slots |'
        print '=================='
        for i in range(1,nNodes+1):
            if not (i in FreeUseNodes or i in physicsNodes):
	continue
            nodeName = 'node'+str(i)
            PrintNodeInfo(nodeName)
    if showUsers:
        qstatOutput = GetOutputFromCommand('qstat -u \"*\"','\n')
        print '====================================================='
        print '| Username    |    Slots    | Queued | Errors | Suspended |'
        print '====================================================='
        for user in physicsUsers+uGradsUsers+facultyUsers+staffUsers:
            userJobs = GetJobsFromUser(user,qstatOutput)
            if userJobs['slots'] == 0 and userJobs['queued'] == 0 and userJobs['errors'] == 0 and userJobs['suspended'] == 0:
	continue
            print '| %9s | %7d | %6d | %6d | %9d |'%(user,userJobs['slots'],userJobs['queued'],userJobs['errors'],userJobs['suspended'])
        
    print 'There are a total of '+str(availableSlots)+' job slots available'
    sys.exit(1)



if __name__ == "__main__":
    main(sys.argv[1:])
