// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "LMAReloadFinishedAnimNotify.h"

void ULMAReloadFinishedAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) 
{
  OnNotifyReloadFinished.Broadcast(MeshComp);
  Super::Notify(MeshComp, Animation);

}
