import numpy as np
import numexpr as ne

def hook2(rampparams, t, etc):
    """
    This function creates a model that fits the HST 'hook' using a rising exponential.

    Parameters
    ----------
    goal:  goal as x -> inf
    m:	   rise exp
    x0:	   time offset
    x:	   Array of time/phase points

    Returns
    -------
    This function returns an array of y values by combining an eclipse and a rising exponential

    Revisions
    ---------
    201-07-09	Kevin Stevenson, UCF  
	            kevin218@knights.ucf.edu
                Original version
    """

    goal    = rampparams[0]
    r0      = rampparams[1]
    r1      = rampparams[2]
    pm      = rampparams[3]
    period  = rampparams[4]
    #framenum = etc[0]
    
    #if len(framenum) == len(t):
    #    istartbatch = np.concatenate((np.where(framenum == 0)[0],[None]))
    #else:
    #    #FINDME: Doesn't work perfectly, needs improvement!
    #    istartbatch = np.concatenate((np.where(framenum == 0)[0]*len(t)/len(framenum),[None]))
    t_batch     = (t-t[0]+0.02) % period
    #for i in range(len(istartbatch)-1):
    #    t_batch[istartbatch[i]:istartbatch[i+1]] = t[istartbatch[i]:istartbatch[i+1]]-t[istartbatch[i]]

    return ne.evaluate('goal + pm*exp(-r0*t_batch + r1)')
