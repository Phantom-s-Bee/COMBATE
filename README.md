# Robô de Combate

Bem-vindo ao repositório do **Phantom Bee** para o nosso robô de combate! Este espaço contém os códigos que controlam o funcionamento do robô, incluindo sistemas de navegação, controle de sensores e comunicação.

---

## 📂 Projetos de Design

O design dos componentes de hardware foi desenvolvido no **Fusion360**. Estes arquivos incluem:  
- **Modelos 3D**  
- **Diagramas Mecânicos**  
- **Esquemas de Circuitos**

Para acessar os arquivos do hardware, é necessário registrar sua conta Autodesk junto à nossa organização. Clique no link abaixo para solicitar acesso:  

[🔗 Arquivos de Hardware no Fusion360](https://veigadealmeida14.autodesk360.com/g/projects/20240824794660206/data/dXJuOmFkc2sud2lwcHJvZDpmcy5mb2xkZXI6Y28uUElTay1DaFpSa2lRZ3p1Q2llWVRMZw)

---

## 🎯 Objetivo

Este repositório foi criado para:  
- Compartilhar o progresso no desenvolvimento do firmware do robô.  
- Promover a colaboração entre membros da equipe e a comunidade.  
- Estimular inovações no campo da robótica.  

---

## 🔧 Requisítos Técnicos

Certifiquesse de que você possui os seguintes programas instalados para poder usar e contribuir para o projeto:

  - [Git](https://git-scm.com/downloads) (Usado no versionamento do projeto)
  - [Fusion360](https://www.autodesk.com/education/edu-software/overview) (Usado para modelagem 3D)
  - [VSCode](https://code.visualstudio.com/Download) (Editor de texto usado para desenvolvimento)
  - [Arduino IDE](https://www.arduino.cc/en/software) (Usado para programar e carregar o micro-controlador)


## 🚀 Como Configurar o Ambiente

### Primeira vez no repositório? Siga os passos abaixo:


1. Abra o terminal e acesse a pasta que deseja colocar o projeto.
2. Execute o seguinte comando para clonar o projeto para o seu computador:
  ```bash
  git clone https://github.com/Phantom-s-Bee/COMBATE.git
  ```

---

## 🔄 Fluxo de Trabalho com Git

### Primeira Vez Usando Git

Se essa for a sua primeira vez usando o git, você tera de fazer algumas configurações.

1. Abra o terminal
2. Execute os seguintes comandos
```bash
git config --global user.name "seuNomeDeUsuario"
git config --global user.email "meu_email@email.com"
```

### Como Subir Atualizações  
Sempre que realizar modificações no código e desejar enviar para o repositório na nuvem:  

1. Abra o terminal e certifique-se de que está no diretório do projeto.  
2. Execute os comandos:  

```bash
git pull
```
Para garantir que nenhum outro membro vez outra modificação no mesmo arquivo.

```bash
git add .
git status  # ou git diff, para verificar alterações
git commit -m "Comentário sobre a alteração"
git push origin main
```
Para fazer o upload das suas alterações.