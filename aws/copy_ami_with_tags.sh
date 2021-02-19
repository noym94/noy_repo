# AMI ID from user ami-07d299040be20fc85
id=$1

# get AMI name
aws ec2 describe-images --region us-east-1 --image-ids $id --query 'Images[].Name'

# get AMI tags
aws ec2 describe-images --region us-east-1 --image-ids $id --query 'Images[].Tags[]'



