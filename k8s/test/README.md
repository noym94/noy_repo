# Jenkins Deployment on MiniKube

This code provides a way to deploy Jenkins on Minikube which has persistent storage. Jenkins will start with pre-configured plugins and will run a pipeline job.
Some notes:

- I decided to use the official Jenkins helm chart for this use-case. For other cases I would probably prefer to write my own helm chart that will setisfy different needs.
- This code doesn't take into account security and secrets management inside k8s, as well as plain-text passwords in the code

## How To Run this Exercise
``` sh
# echo "Creating jenkins-pv PVC"
> kubectl create -f pvc.yaml
# echo "Allow default users permissions"
> kubectl create clusterrolebinding serviceaccounts-cluster-admin --clusterrole=cluster-admin --group=system:serviceaccounts
# echo "Installing Jenkins using Helm"
> helm install -f jenkins.yaml noy jenkins/jenkins
```
Wait until Jenkins is running. Can use:
``` sh
while [[ $(kubectl get pods --field-selector=status.phase=Running | grep -v NAME | grep 2/2 | awk '{print $1}') != "noy-jenkins-0" ]]
do
  echo "Waiting for Jenkins to start..."
  sleep 1
done
```
``` sh
# echo "Get Jenkins Initial Admin Password"
> kubectl exec --namespace default -it svc/noy-jenkins -c jenkins -- /bin/cat /run/secrets/chart-admin-password && echo
# echo "Expose Jenkins Service on port 8080"
> echo http://127.0.0.1:8080
> kubectl --namespace default port-forward svc/noy-jenkins 8080:8080 &
# echo "Enable reverse-proxy"
> kubectl proxy --port=8081 &
# echo "Print k8s Cluster IP"
> curl http://127.0.0.1:8081/api | jq .serverAddressByClientCIDRs[].serverAddress
```
## Manual Steps
Login to Jenkins on localhost:8080 and add the ip printed from the previous command to the k8s cloud configuration under manage-jenkins.
Copy-Paste the pipeline file content to a new pipeline job and execute.
The pipeline job will build and deploy sample-app on k8s. 

## After the webapp is running, run the following commands to export the application on port 8088
``` sh
> export POD_NAME=$(kubectl get pods --namespace gruntwork -l "app.kubernetes.io/name=webapp,app.kubernetes.io/instance=my-webapp" -o jsonpath="{.items[0].metadata.name}")
echo "Visit http://127.0.0.1:8088 to use your application"
kubectl --namespace gruntwork port-forward $POD_NAME 8088:80
```