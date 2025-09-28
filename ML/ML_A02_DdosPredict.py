#!/usr/bin/python3
import sys
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import json
### by HealthyStrongMuslim i.e Musaab
# get csv for training from CLI 1st argument
csvs = sys.argv[1]
# get input for prediction from CLI 2nd argument (optional)
pltShowArg = sys.argv[2].lower() == 'y' if len(sys.argv) > 2 else False  # Set to True for blocking plots
new_input = sys.argv[3] if len(sys.argv) > 3 else None
if not csvs or not pltShowArg in [True, False]:
  print("""Usage: ./ML_A02_DdosPredict.py <path_to_csv> <show_plots:y/n> [optional_new_input_json]
  Examples:
    ./ML_A02_DdosPredict.py ./data/UNSW_NB15.csv n
    ./ML_A02_DdosPredict.py ./machineLearningCVE/Friday-WorkingHours-Afternoon-DDos.pcap_ISCX.csv n '{"Total Fwd Packets":100,"Total Backward Packets":80,"Fwd Packet Length Mean":30,"Flow Duration":2000,"Average Packet Size":500}'
    ./ML_A02_DdosPredict.py ./machineLearningCVE/Friday-WorkingHours-Afternoon-DDos.pcap_ISCX.csv y '{"Total Fwd Packets":100000,"Total Backward Packets":80,"Fwd Packet Length Mean":30,"Flow Duration":2000,"Average Packet Size":500}'
  Note: Ensure the CSV file path is correct and the JSON input has all required fields.
  Built for csv datasets from http://cicresearch.ca/CICDataset/CIC-IDS-2017/Dataset/CIC-IDS-2017/CSVs/
    (description at https://www.unb.ca/cic/datasets/ids-2017.html)""")
  sys.exit(1)
# Q1
# 1. Load CSV into DataFrame
df = pd.read_csv(csvs)
# 2. Inspect dataset
print("Shape:", df.shape)
df.columns = df.columns.str.strip()  # Removes leading/trailing spaces from column names
df=df[["Total Backward Packets","Total Fwd Packets",
        "Fwd Packet Length Mean","Flow Duration",
        "Average Packet Size","Label"]]
print("Data Types:\n", df.dtypes)
print("Missing values:")
for col in df.columns:
    if df[col].isnull().any():
        print(f"'{col}': {df[col].isnull().sum()} missing values")
df["Label"]=df["Label"].map({'BENIGN': 0, 'DDoS': 1})
# 3. Correlation heatmap
plt.figure(figsize=(10, 6))
sns.heatmap(df.corr(), cmap="coolwarm", annot=False)
plt.title("Feature Correlation Heatmap")
plt.show(block=pltShowArg)
# ==========================

# Q2
# 1. Handle missing values (fill numeric with median)
df.fillna(df.median(numeric_only=True), inplace=True)
# 2. Outlier removal using IQR for two columns
for col in ["Total Fwd Packets", "Flow Duration"]:
  Q1 = df[col].quantile(0.25)
  Q3 = df[col].quantile(0.75)
  IQR = Q3 - Q1
  df = df[(df[col] >= Q1 - 1.5*IQR) & (df[col] <= Q3 + 1.5*IQR)]
# 3. Feature engineering: packet_ratio
df["packet_ratio"] = df["Total Fwd Packets"] / (df["Total Backward Packets"] + 1)
# ==========================

# Q3
from statsmodels.stats.outliers_influence import variance_inflation_factor
from sklearn.preprocessing import StandardScaler
# Select predictors
X = df[["Total Backward Packets", #dropped "Total Fwd Packets" to reduce multicollinearity
        "Fwd Packet Length Mean","Flow Duration",
        "Average Packet Size","packet_ratio"]]
# Calculate VIF
vif_data = pd.DataFrame()
vif_data["feature"] = X.columns
vif_data["VIF"] = [variance_inflation_factor(X.values, i) for i in range(X.shape[1])]
intensity = [46,154,190,226,220,214,208,202,196]
maxvif = 23.4 #vif_data['VIF'].max()
print("Feature:\t\t\tVIF")
for index, row in vif_data.iterrows():
    print(f"\33[38;5;{intensity[int((row['VIF']/maxvif)*len(intensity))]}m{row['feature']}:\t{row['VIF']:<.3f}\33[0m")
# Standardize features
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)
print("Before scaling:\n", X.describe().T[['mean','std']])
print("After scaling (first 5 rows):\n", X_scaled[:5])
# ==========================

# Q4
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
import statsmodels.api as sm
# Target variable 
y = df["Label"]
# Train/test split
X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=0.2, random_state=42, stratify=y)
# 1. Scikit-learn model
print(X_train.shape,"\n", y_train.shape)
lr = LogisticRegression(max_iter=1000)
if len(y.unique()) < 2: raise ValueError("Dataset contains only one class after preprocessing. Cannot train logistic regression.")
lr.fit(X_train, y_train)
# 2. Statsmodels OLS for regression summary
X_train_const = sm.add_constant(X_train)
logR = sm.Logit(y_train, X_train_const).fit()
print(logR.summary())
# ==========================

# Q5
from sklearn.metrics import mean_absolute_error, mean_squared_error
# Predictions
y_pred = lr.predict(X_test)
# 1. Residuals plot
residuals = y_test - y_pred
plt.scatter(y_pred, residuals)
plt.axhline(0, color='red')
plt.xlabel("Fitted values")
plt.ylabel("Residuals")
plt.title("Residuals vs Fitted")
plt.show(block=pltShowArg)
# 2. Q-Q plot
sm.qqplot(residuals, line="45")
plt.title("Q-Q Plot of Residuals")
plt.show(block=pltShowArg)
# 3. Metrics
mae = mean_absolute_error(y_test, y_pred)
rmse = mean_squared_error(y_test, y_pred)
print(f"Error magnitude:\n  MAE:\t{mae:<.4f}")
print(f"  RMSE:\t{rmse:<.4f}")
# ==========================

# Q7 – Automation Challenge
# predict on input if provided
if new_input:
  try:
    # Parse new input JSON from CLI
    record = pd.DataFrame([json.loads(new_input)])
    # Feature engineering: packet_ratio
    record["packet_ratio"] = record["Total Fwd Packets"] / (record["Total Backward Packets"] + 1)
    # Keep only the columns used in training
    record_X = record[["Total Backward Packets",
                        "Fwd Packet Length Mean","Flow Duration",
                        "Average Packet Size","packet_ratio"]]
    # Scale using the same scaler fit earlier
    record_scaled = scaler.transform(record_X)
    # Predict
    pred = lr.predict(record_scaled)[0]
    pred_proba = lr.predict_proba(record_scaled)[0][1]
    # Output
    label = "DDoS" if pred == 1 else "BENIGN"
    print(f"\nInput: \33[33m{new_input}\n",\
          f"\33[38;5;{intensity[int((pred_proba)*(len(intensity)-1))]}mPrediction: {label} (probability of DDoS = {pred_proba:.4f})\33[0m")
  except Exception as e:
    print("Error in processing new input:", e)
