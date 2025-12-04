#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- TYPES ---------- */
typedef struct {
    int idFournisseur;
    char Nom[50];
    char adresse[50];
    char telephone[20];
    char email[50];
} Fournisseur;

typedef struct {
    int idProduit;
    char designation[30];
    char Nom[50];
    float prix;
    int quantite;
    int idFournisseur;
} Produit;

/* ---------- PROTOTYPES ---------- */
/* Fournisseur */
void ajouterFournisseur();
void supprimerFournisseur();
void modifierFournisseur();
void afficherFournisseur();
void consulterProfilFournisseur();

/* Produit */
void ajouterProduit();
void supprimerProduit();
void modifierProduit();
void afficherProduit();
void fairePromos();
void pourcentageAvecAdmin();
void calculPartLivraison();
void calculTotal();
void consulterStock();
void MettreAJourStock();

/* ---------- IMPLEMENTATION ---------- */

/* ----------------- FOURNISSEUR ----------------- */
int idFournisseurExiste(int idRecherche)
{
    FILE *f = fopen("fournisseur.dat", "rb"); // lire le fichier binaire
    if (!f) return 0; // si fichier inexistant, ID n'existe pas

    Fournisseur F;
    while (fread(&F, sizeof(Fournisseur), 1, f) == 1)
    {
        if (F.idFournisseur == idRecherche) {
            fclose(f);
            return 1; // ID existe déjà
        }
    }

    fclose(f);
    return 0; // ID unique
}

void ajouterFournisseur()
{
    Fournisseur F;

    printf("Entrez l'ID du fournisseur : ");
    scanf("%d", &F.idFournisseur);
    while(getchar() != '\n'); // vider le buffer après scanf

    // Vérification de l'unicité
    if (idFournisseurExiste(F.idFournisseur))
    {
        printf("❌ Cet ID existe déjà ! Fournisseur non ajouté.\n");
        return;
    }

    printf("Entrez le nom du fournisseur : ");
    fgets(F.Nom, sizeof(F.Nom), stdin);
    F.Nom[strcspn(F.Nom, "\n")] = '\0'; // enlever le \n final

    printf("Entrez l'adresse du fournisseur : ");
    fgets(F.adresse, sizeof(F.adresse), stdin);
    F.adresse[strcspn(F.adresse, "\n")] = '\0';

    printf("Entrez le numéro de téléphone : ");
    fgets(F.telephone, sizeof(F.telephone), stdin);
    F.telephone[strcspn(F.telephone, "\n")] = '\0';

    printf("Entrez l'adresse email : ");
    fgets(F.email, sizeof(F.email), stdin);
    F.email[strcspn(F.email, "\n")] = '\0';

    // Ouverture du fichier binaire en mode ajout
    FILE *f = fopen("fournisseur.dat", "ab");
    if (!f) {
        printf("Erreur fichier !\n");
        return;
    }

    fwrite(&F, sizeof(Fournisseur), 1, f);
    fclose(f);

    printf("✔ Fournisseur ajouté avec succès.\n");
}

void supprimerFournisseur() {
    FILE *fic = fopen("fournisseur.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fic || !temp) {
        printf("Impossible d'ouvrir le fichier!\n");
        if (fic) fclose(fic);
        if (temp) fclose(temp);
        return;
    }

    int idSupprimer;
    int trouve = 0;
    Fournisseur f;

    printf("\n== SUPPRESSION D'UN FOURNISSEUR ==\n");
    printf("Entrez l'ID du fournisseur à supprimer : ");
    if (scanf("%d", &idSupprimer) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }

    while (fread(&f, sizeof(Fournisseur), 1, fic) == 1) {
        if (f.idFournisseur == idSupprimer) {
            trouve = 1;
            printf("✔ Le fournisseur '%s' a été supprimé.\n", f.Nom);
            /* ne pas écrire dans temp */
        } else {
            fwrite(&f, sizeof(Fournisseur), 1, temp);
        }
    }

    fclose(fic);
    fclose(temp);

    remove("fournisseur.dat");
    rename("temp.dat", "fournisseur.dat");

    if (!trouve) {
        printf("❌ Aucun fournisseur avec l'ID %d n'a été trouvé.\n", idSupprimer);
    }
}

void modifierFournisseur() {
    FILE *fic = fopen("fournisseur.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if(!fic || !temp){
        printf("Impossible d'ouvrir le fichier !\n");
        if(fic) fclose(fic);
        if(temp) fclose(temp);
        return;
    }

    int idModifier;
    Fournisseur F;
    int trouve = 0;
    char rep;

    printf("Entrez l'ID du fournisseur : ");
    scanf("%d", &idModifier);
    while(getchar() != '\n');

    while(fread(&F, sizeof(Fournisseur), 1, fic) == 1){
        if(F.idFournisseur == idModifier){
            trouve = 1;
            printf("Fournisseur trouvé : %s\n", F.Nom);

            printf("Modifier nom ? (o/n) : "); scanf(" %c",&rep); while(getchar()!='\n');
            if(rep=='o'||rep=='O'){ printf("Nouveau nom : "); fgets(F.Nom,sizeof(F.Nom),stdin); F.Nom[strcspn(F.Nom,"\n")]=0; }

            printf("Modifier adresse ? (o/n) : "); scanf(" %c",&rep); while(getchar()!='\n');
            if(rep=='o'||rep=='O'){ printf("Nouvelle adresse : "); fgets(F.adresse,sizeof(F.adresse),stdin); F.adresse[strcspn(F.adresse,"\n")]=0; }

            printf("Modifier téléphone ? (o/n) : "); scanf(" %c",&rep); while(getchar()!='\n');
            if(rep=='o'||rep=='O'){ printf("Nouveau téléphone : "); fgets(F.telephone,sizeof(F.telephone),stdin); F.telephone[strcspn(F.telephone,"\n")]=0; }

            printf("Modifier email ? (o/n) : "); scanf(" %c",&rep); while(getchar()!='\n');
            if(rep=='o'||rep=='O'){ printf("Nouvel email : "); fgets(F.email,sizeof(F.email),stdin); F.email[strcspn(F.email,"\n")]=0; }

            printf("✔ Modifications effectuées.\n");
        }

        fwrite(&F, sizeof(Fournisseur), 1, temp);
    }

    fclose(fic);
    fclose(temp);

    remove("fournisseur.dat");
    rename("temp.dat","fournisseur.dat");

    if(!trouve) printf("❌ Aucun fournisseur avec l'ID %d trouvé.\n", idModifier);
}
void afficherFournisseurs() {
    FILE *fic = fopen("fournisseur.dat", "rb");
    if(!fic){ printf("❌ Aucun fournisseur enregistré.\n"); return; }

    Fournisseur F;
    printf("ID    Nom                  Adresse                Téléphone        Email\n");
    printf("----------------------------------------------------------------------------\n");

    while(fread(&F, sizeof(Fournisseur), 1, fic) == 1){
        printf("%-5d %-20s %-20s %-15s %-30s\n", F.idFournisseur,F.Nom,F.adresse,F.telephone,F.email);
    }

    fclose(fic);
}
void consulterProfilFournisseur() {
    FILE *fic = fopen("fournisseur.dat", "rb");
    if (!fic) {
        printf("Aucun fournisseur enregistré (fichier introuvable).\n");
        return;
    }

    int idRecherche;
    Fournisseur f;
    int trouve = 0;

    printf("\n=== CONSULTER PROFIL FOURNISSEUR ===\n");
    printf("Entrez votre ID fournisseur : ");
    if (scanf("%d", &idRecherche) != 1) {
        printf("ID invalide.\n");
        while (getchar() != '\n');
        fclose(fic);
        return;
    }

    while (fread(&f, sizeof(Fournisseur), 1, fic) == 1) {
        if (f.idFournisseur == idRecherche) {
            trouve = 1;
            printf("\n--- VOTRE PROFIL ---\n");
            printf("ID        : %d\n", f.idFournisseur);
            printf("Nom       : %s\n", f.Nom);
            printf("Adresse   : %s\n", f.adresse);
            printf("Téléphone : %s\n", f.telephone);
            break;
        }
    }

    if (!trouve)
        printf("❌ Aucun fournisseur trouvé avec l'ID %d.\n", idRecherche);

    fclose(fic);
}

/* ----------------- PRODUIT ----------------- */
int idProduitExiste(int id)
{
    FILE *f = fopen("produits.txt", "r");
    if (!f) return 0;

    Produit P;
    while (fscanf(f, "%d %s %f %d %d",
                  &P.idProduit, P.designation, &P.prix,
                  &P.quantite, &P.idFournisseur) == 5)
    {
        if (P.idProduit == id) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void ajouterProduit()
{
    Produit P;

    printf("Entrez l'ID du produit : ");
    scanf("%d", &P.idProduit);
    while(getchar() != '\n');

    // Vérification de l'unicité
    if (idProduitExiste(P.idProduit))
    {
        printf("❌ Cet ID existe déjà ! Produit non ajouté.\n");
        return;
    }

    printf("Entrez la désignation : ");
    fgets(P.designation, sizeof(P.designation), stdin);
    P.designation[strcspn(P.designation, "\n")] = '\0';

    printf("Entrez le prix : ");
    scanf("%f", &P.prix);
    while(getchar() != '\n');

    printf("Entrez la quantité : ");
    scanf("%d", &P.quantite);
    while(getchar() != '\n');

    printf("Entrez l'ID du fournisseur : ");
    scanf("%d", &P.idFournisseur);
    while(getchar() != '\n');

    // ouverture du fichier binaire en mode ajout
    FILE *f = fopen("produit.dat", "ab");
    if (!f) {
        printf("Erreur fichier !\n");
        return;
    }

    fwrite(&P, sizeof(Produit), 1, f);
    fclose(f);

    printf("✔ Produit ajouté avec succès.\n");
}

void supprimerProduit() {
    FILE *fic = fopen("produit.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fic || !temp) {
        printf("Impossible d'ouvrir le fichier!\n");
        if (fic) fclose(fic);
        if (temp) fclose(temp);
        return;
    }

    int idSupprimer, trouve = 0;
    Produit p;

    printf("\n== SUPPRESSION D'UN PRODUIT ==\n");
    printf("Entrez l'ID du produit à supprimer : ");
    if (scanf("%d", &idSupprimer) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idSupprimer) {
            trouve = 1;
            printf("✔ Le produit %d a été supprimé.\n", idSupprimer);
            /* ne pas écrire dans temp */
        } else {
            fwrite(&p, sizeof(Produit), 1, temp);
        }
    }

    fclose(fic);
    fclose(temp);

    remove("produit.dat");
    rename("temp.dat", "produit.dat");

    if (!trouve)
        printf("❌ Aucun produit avec l'ID %d n'a été trouvé.\n", idSupprimer);
}

void modifierProduit() {
    FILE *fic = fopen("produit.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fic || !temp) {
        printf("Impossible d'ouvrir le fichier!\n");
        if (fic) fclose(fic);
        if (temp) fclose(temp);
        return;
    }

    int idModifier, trouve = 0;
    Produit p;
    char rep;

    printf("\n== MODIFICATION DU PRODUIT ==\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idModifier) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idModifier) {
            trouve = 1;
            printf("\nProduit trouvé : %s (ID %d)\n", p.Nom, p.idProduit);

            /* Modifier Nom */
            printf("Voulez-vous modifier le nom du produit ? (o/n) : ");
            scanf(" %c", &rep);
            while (getchar() != '\n');
            if (rep == 'o' || rep == 'O') {
                printf("Entrez le nouveau nom : ");
                fgets(p.Nom, sizeof(p.Nom), stdin);
                p.Nom[strcspn(p.Nom, "\n")] = '\0';
            }

            /* Modifier designation */
            printf("Voulez-vous modifier la designation du produit ? (o/n) : ");
            scanf(" %c", &rep);
            while (getchar() != '\n');
            if (rep == 'o' || rep == 'O') {
                printf("Entrez la nouvelle designation : ");
                fgets(p.designation, sizeof(p.designation), stdin);
                p.designation[strcspn(p.designation, "\n")] = '\0';
            }

            /* Modifier prix */
            printf("Voulez-vous modifier le prix ? (o/n) : ");
            scanf(" %c", &rep);
            while (getchar() != '\n');
            if (rep == 'o' || rep == 'O') {
                printf("Veuillez entrer le nouveau prix : ");
                if (scanf("%f", &p.prix) != 1) {
                    printf("Prix invalide, annulation modification prix.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                }
            }

            /* Modifier quantite */
            printf("Voulez-vous modifier la quantité ? (o/n) : ");
            scanf(" %c", &rep);
            while (getchar() != '\n');
            if (rep == 'o' || rep == 'O') {
                printf("Entrez la nouvelle quantité : ");
                if (scanf("%d", &p.quantite) != 1) {
                    printf("Quantité invalide, annulation modification quantité.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                }
            }

            /* Modifier idFournisseur */
            printf("Voulez-vous modifier l'ID du fournisseur associé ? (o/n) : ");
            scanf(" %c", &rep);
            while (getchar() != '\n');
            if (rep == 'o' || rep == 'O') {
                printf("Entrez le nouvel ID fournisseur : ");
                if (scanf("%d", &p.idFournisseur) != 1) {
                    printf("ID fournisseur invalide, annulation.\n");
                    while (getchar() != '\n');
                } else {
                    while (getchar() != '\n');
                }
            }

            printf("✔ Modifications effectuées.\n");
        }

        fwrite(&p, sizeof(Produit), 1, temp);
    }

    fclose(fic);
    fclose(temp);

    remove("produit.dat");
    rename("temp.dat", "produit.dat");

    if (!trouve)
        printf("❌ Aucun produit n'est identifié avec l'ID %d.\n", idModifier);
}

void afficherProduit() {
    FILE *fic = fopen("produit.dat", "rb");
    if (!fic) {
        printf("Aucun produit enregistré (fichier introuvable).\n");
        return;
    }

    Produit p;
    printf("\n== LISTE DES PRODUITS ==\n");
    printf("%-5s %-20s %-25s %-10s %-10s %-10s\n", "ID", "Nom", "Designation", "Prix", "Quantite", "IdFourn");
    printf("--------------------------------------------------------------------------------------\n");

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        printf("%-5d %-20s %-25s %-10.2f %-10d %-10d\n",
               p.idProduit, p.Nom, p.designation, p.prix, p.quantite, p.idFournisseur);
    }

    fclose(fic);
}

/* Appliquer promotion (pourcentage sur prix) */
void fairePromos() {
    FILE *fic = fopen("produit.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fic || !temp) {
        printf("Impossible d'ouvrir le fichier!\n");
        if (fic) fclose(fic);
        if (temp) fclose(temp);
        return;
    }

    int idPromo;
    Produit p;
    int trouve = 0;
    float promo;

    printf("\n=== BENEFICIER DES PROMOTIONS ===\n");
    printf("Entrez l'ID du produit : ");
    if (scanf("%d", &idPromo) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idPromo) {
            trouve = 1;
            printf("Combien de pourcentage voulez-vous appliquer sur le produit %d ? ", idPromo);
            if (scanf("%f", &promo) != 1) {
                printf("Pourcentage invalide. Promotion annulée pour ce produit.\n");
                while (getchar() != '\n');
            } else {
                while (getchar() != '\n');
                float reduction = p.prix * (promo / 100.0f);
                p.prix -= reduction;
                printf("✔ Nouveau prix du produit %d : %.2f DH\n", idPromo, p.prix);
            }
        }
        fwrite(&p, sizeof(Produit), 1, temp);
    }

    fclose(fic);
    fclose(temp);

    remove("produit.dat");
    rename("temp.dat", "produit.dat");

    if (!trouve)
        printf("❌ Aucun produit d'ID %d n'a été trouvé.\n", idPromo);
    else
        printf("✔ La promotion a été appliquée (si le produit existait).\n");
}

/* Pourcentage admin sur le prix (affiche part admin et part fournisseur) */
void pourcentageAvecAdmin() {
    FILE *fic = fopen("produit.dat", "rb");
    if (!fic) {
        printf("Aucun produit enregistré (fichier introuvable).\n");
        return;
    }

    int idProduit;
    float pourcentage;
    Produit p;
    int trouve = 0;

    printf("\n=== POURCENTAGE ADMIN ===\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idProduit) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    printf("Veuillez entrer le pourcentage de l'Admin : ");
    if (scanf("%f", &pourcentage) != 1) {
        printf("Pourcentage invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idProduit) {
            trouve = 1;
            float reduction = p.prix * (pourcentage / 100.0f);
            float prix_fournisseur = p.prix - reduction;
            printf("\nPart de l'admin (%.2f%%) : %.2f DH\n", pourcentage, reduction);
            printf("Part du fournisseur : %.2f DH\n", prix_fournisseur);
            break;
        }
    }

    if (!trouve)
        printf("❌ Aucun produit avec l'ID %d n'a été trouvé.\n", idProduit);

    fclose(fic);
}

/* Calcul part livraison (affiche frais livraison) */
void calculPartLivraison() {
    FILE *fic = fopen("produit.dat", "rb");
    if (!fic) {
        printf("Aucun produit enregistré (fichier introuvable).\n");
        return;
    }

    int idProduit;
    float tauxLivraison;
    Produit p;
    int trouve = 0;

    printf("\n=== LA PART DE LA LIVRAISON ===\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idProduit) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    printf("Veuillez entrer le pourcentage de livraison : ");
    if (scanf("%f", &tauxLivraison) != 1) {
        printf("Pourcentage invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idProduit) {
            trouve = 1;
            float fraisLivraison = p.prix * (tauxLivraison / 100.0f);
            printf("\nProduit : %s\nPrix produit : %.2f DH\nFrais livraison calculé : %.2f DH\n",
                   p.Nom, p.prix, fraisLivraison);
            break;
        }
    }

    if (!trouve)
        printf("❌ Produit introuvable.\n");

    fclose(fic);
}

/* Calcul total à payer (produit + livraison - part admin) */
void calculTotal() {
    FILE *fic = fopen("produit.dat", "rb");
    if (!fic) {
        printf("Aucun produit enregistré (fichier introuvable).\n");
        return;
    }

    int idProduit;
    float pourcentageAdmin, tauxLivraison;
    Produit p;
    int trouve = 0;

    printf("\n=== CALCUL TOTAL A PAYER ===\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idProduit) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    printf("Entrez le pourcentage de l'Admin : ");
    if (scanf("%f", &pourcentageAdmin) != 1) {
        printf("Pourcentage admin invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    printf("Entrez le pourcentage de livraison : ");
    if (scanf("%f", &tauxLivraison) != 1) {
        printf("Pourcentage livraison invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idProduit) {
            trouve = 1;
            float fraisLivraison = p.prix * (tauxLivraison / 100.0f);
            float total = p.prix + fraisLivraison;
            float reductionAdmin = total * (pourcentageAdmin / 100.0f);
            float partFournisseur = total - reductionAdmin;
            printf("\n--- Récapitulatif du produit ---\n");
            printf("Produit : %s\n", p.Nom);
            printf("Prix du produit : %.2f DH\n", p.prix);
            printf("Frais livraison (%.2f%%) : %.2f DH\n", tauxLivraison, fraisLivraison);
            printf("Total (produit + livraison) : %.2f DH\n", total);
            printf("Part Admin (%.2f%%) : %.2f DH\n", pourcentageAdmin, reductionAdmin);
            printf("Part Fournisseur : %.2f DH\n", partFournisseur);
            break;
        }
    }

    if (!trouve)
        printf("❌ Produit introuvable.\n");

    fclose(fic);
}

/* Consulter stock (quantité d'un produit) */
void consulterStock() {
    FILE *fic = fopen("produit.dat", "rb");
    if (!fic) {
        printf("Aucun produit enregistré (fichier introuvable).\n");
        return;
    }

    int idProduit;
    Produit p;
    int trouve = 0;

    printf("\n=== CONSULTER STOCK ===\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idProduit) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idProduit) {
            trouve = 1;
            printf("La quantité restante est : %d\n", p.quantite);
            break;
        }
    }

    if (!trouve)
        printf("❌ Aucun produit avec l'ID %d n'a été trouvé.\n", idProduit);

    fclose(fic);
}

/* Mettre à jour le stock après un achat (décrémente la quantité) */
void MettreAJourStock() {
    FILE *fic = fopen("produit.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fic || !temp) {
        printf("Impossible d'ouvrir le fichier!\n");
        if (fic) fclose(fic);
        if (temp) fclose(temp);
        return;
    }

    int idProduit, qteAchetee;
    Produit p;
    int trouve = 0;

    printf("\n=== MISE A JOUR DU STOCK APRES ACHAT ===\n");
    printf("Veuillez entrer l'ID du produit : ");
    if (scanf("%d", &idProduit) != 1) {
        printf("ID invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }

    printf("Entrez la quantité achetée : ");
    if (scanf("%d", &qteAchetee) != 1) {
        printf("Quantité invalide.\n");
        fclose(fic);
        fclose(temp);
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(Produit), 1, fic) == 1) {
        if (p.idProduit == idProduit) {
            trouve = 1;
            if (p.quantite < qteAchetee) {
                printf("Stock insuffisant! Stock actuel : %d\n", p.quantite);
                /* on écrit quand même l'enregistrement inchangé */
                fwrite(&p, sizeof(Produit), 1, temp);
            } else {
                p.quantite -= qteAchetee;
                printf("Achat enregistré! Nouveau stock : %d\n", p.quantite);
                fwrite(&p, sizeof(Produit), 1, temp);
            }
        } else {
            fwrite(&p, sizeof(Produit), 1, temp);
        }
    }

    fclose(fic);
    fclose(temp);

    remove("produit.dat");
    rename("temp.dat", "produit.dat");

    if (!trouve)
        printf("❌ Produit avec ID %d introuvable.\n", idProduit);
}

/* ---------- MAIN ---------- */
int main() {
    int choix;

    do {
        printf("\n==============================\n");
        printf("     MENU PRINCIPAL\n");
        printf("==============================\n");
        printf("1. Ajouter un fournisseur\n");
        printf("2. Supprimer un fournisseur\n");
        printf("3. Modifier un fournisseur\n");
        printf("4. Afficher les fournisseurs\n");
        printf("5. Consulter profil fournisseur\n");
        printf("6. Ajouter un produit\n");
        printf("7. Supprimer un produit\n");
        printf("8. Modifier un produit\n");
        printf("9. Afficher les produits\n");
        printf("10. Faire une promotion sur un produit\n");
        printf("11. Calcul du pourcentage administrateur\n");
        printf("12. Calcul part livraison\n");
        printf("13. Calcul total à payer\n");
        printf("14. Consulter stock\n");
        printf("15. Mettre à jour stock après achat\n");
        printf("0. Quitter\n");
        printf("==============================\n");
        printf("Entrez votre choix : ");

        if (scanf("%d", &choix) != 1) {
            printf("Choix invalide.\n");
            while (getchar() != '\n'); // vider le buffer
            choix = -1;
            continue;
        }
        while (getchar() != '\n'); // consommer le '\n'

        switch (choix) {
            case 1: ajouterFournisseur(); break;
            case 2: supprimerFournisseur(); break;
            case 3: modifierFournisseur(); break;
            case 4: afficherFournisseurs(); break; // corrigé ici
            case 5: consulterProfilFournisseur(); break;
            case 6: ajouterProduit(); break;
            case 7: supprimerProduit(); break;
            case 8: modifierProduit(); break;
            case 9: afficherProduit(); break;
            case 10: fairePromos(); break;
            case 11: pourcentageAvecAdmin(); break;
            case 12: calculPartLivraison(); break;
            case 13: calculTotal(); break;
            case 14: consulterStock(); break;
            case 15: MettreAJourStock(); break;
            case 0: printf("Au revoir !\n"); break;
            default: printf("Choix invalide, veuillez réessayer.\n"); break;
        }
    } while (choix != 0);

    return 0;
}






