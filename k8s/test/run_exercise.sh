#!/bin/bash
echo "Creating general Namespace"
kubectl create -f namespace.yaml

echo "Creating jenkins-pv PVC"
kubectl create -f pvc.yaml

echo "Installing Jenkins using Helm"
helm install -f jenkins.yaml noy jenkins/jenkins

echo "Copy Pipeline Config to jenkins home"
kubectl cp pipeline noy-jenkins-0:/var/jenkins_home/jobs/pipeline/config.xml -c jenkins

echo "Allow default users permissions"
kubectl create clusterrolebinding serviceaccounts-cluster-admin --clusterrole=cluster-admin --group=system:serviceaccounts

echo "Watiting for Jenkins to startup"
while (kubectl get pods --field-selector=status.phase=Running == 0)
do
  sleep
done

echo "Get Jenkins Initial Admin Password"
kubectl exec --namespace default -it svc/noy-jenkins -c jenkins -- /bin/cat /run/secrets/chart-admin-password && echo

echo "Expose Jenkins Service on port 8080"
echo http://127.0.0.1:8080
kubectl --namespace default port-forward svc/noy-jenkins 8080:8080 &

echo "Enable reverse-proxy"
kubectl proxy --port=8081 &

echo "Print k8s Cluster IP"
curl http://127.0.0.1:8081/api | jq .serverAddressByClientCIDRs[].serverAddress

echo "Export sample-app"
export POD_NAME=$(kubectl get pods --namespace gruntwork -l "app.kubernetes.io/name=webapp,app.kubernetes.io/instance=my-webapp" -o jsonpath="{.items[0].metadata.name}")
echo "Visit http://127.0.0.1:8088 to use your application"
kubectl --namespace gruntwork port-forward $POD_NAME 8088:80