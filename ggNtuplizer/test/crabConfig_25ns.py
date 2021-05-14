if __name__ == '__main__':

# Usage : python crabConfig.py (to create jobs)
#         ./multicrab -c status -d <work area> (to check job status)

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    from CRABClient.UserUtilities import config
    config = config()
    
    from multiprocessing import Process

    # Common configuration

    config.General.workArea     = 'crab_projects_ntuples'
    config.General.transferLogs = False
    config.JobType.pluginName   = 'Analysis' # PrivateMC
    config.JobType.psetName     = 'run_mc2017_110X.py'
    #config.JobType.inputFiles   = ['Summer16_23Sep2016V4_MC_L2Relative_AK8PFchs.txt', 'Summer16_23Sep2016V4_MC_L3Absolute_AK8PFchs.txt', 'Summer16_23Sep2016V4_MC.db']
    config.JobType.sendExternalFolder = True
    config.Data.inputDBS        = 'global'    
    config.Data.splitting       = 'FileBased' # EventBased, FileBased, LumiBased (1 lumi ~= 300 events)
    config.Data.totalUnits      = -1
    config.Data.publication     = False
    config.Site.storageSite     = 'T2_CH_CERN'

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers

    # dataset dependent configuration
    submitVersion = "job_ChargeMisID_DY3"
    config.General.requestName = 'job_ChargeMisID_DY3'
    config.Data.unitsPerJob    = 1
    config.Data.inputDataset   = '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIISummer19UL17MiniAOD-106X_mc2017_realistic_v6-v2/MINIAODSIM'
    config.Data.outLFNDirBase  = '/store/group/phys_egamma/akapoor/ChargeMisID/%s' % submitVersion
    p = Process(target=submit, args=(config,))
    p.start()
    p.join()




