
PROMPT='>>>>>>>>>>>'
MY_EMAIL=lvyanbing9@gmail.com
MY_NAME=lvbing

# show----------------------------------
USER=$(users)
echo "$PROMPT user:"
echo $USER

echo "$PROMPT you email:"
echo $MY_EMAIL

# ssh----------------------------------
echo "$PROMPT create SSH key:"
ssh-keygen -t rsa -C "$MY_EMAIL"

echo "$PROMPT id_rsa:"
cat /home/$USER/.ssh/id_rsa
echo "$PROMPT id_rsa.pub:"
cat /home/$USER/.ssh/id_rsa.pub

# git----------------------------------
GIT_USER_NAME=$MY_NAME
GIT_USER_EMAIL=$MY_EMAIL
GIT_EDITIR='vim'
GIT_DIFF='vimdif'
echo $GIT_USER_NAME

echo "$PROMPT install git:"
sudo apt install git
git --version

echo "$PROMPT test ssh link github.com:"
ssh -T git@github.com

# --system(/etc/gitconfig)  --global(~/.gitconfig)  (.git/config)
echo "$PROMPT git user name: $GIT_USER_NAME"
git config --global user.name $GIT_USER_NAME
echo "$PROMPT git user email: $GIT_USER_EMAIL"
git config --global user.email $GIT_USER_EMAIL

echo "$PROMPT git editor is: $GIT_EDITIR"
git config --global core.editor $GIT_EDITIR

echo "$PROMPT git diff is: $GIT_DIFF"
git config --global merge.tool $GIT_DIFF

echo "$PROMPT git config color: ..."
git config --global color.ui auto
git config --global color.status auto
git config --global color.branch auto
git config --global color.diff auto
git config --global color.interactive auto

echo "$PROMPT git config cmd alias: ..."
git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
git config --global alias.st "status"

echo "$PROMPT git change data time format: ..."
git config --global  log.date iso
git config --global  log.date format:'%Y-%m-%d %H:%M:%S'

echo "$PROMPT show git config list:"
git config --list
# git config --global --list
# git config --system --list

